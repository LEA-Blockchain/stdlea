const { console } = require('inspector');

const fs = require('fs').promises;

const print = (() => {
    const colors = {
        red: { ansi: 196, css: "red" },
        orange: { ansi: 208, css: "orange" },
        green: { ansi: 46, css: "green" },
        blue: { ansi: 33, css: "blue" },
    };

    const printMessage = (msg, { ansi, css }) => {
        if (typeof process !== 'undefined' && process.stdout?.write) {
            process.stdout.write(`\x1b[38;5;${ansi}m${msg}\x1b[0m`);
        } else {
            console.log(`%c${msg}`, `color: ${css}`);
        }
    };

    const api = {};
    for (const [name, cfg] of Object.entries(colors)) {
        api[name] = msg => printMessage(msg, cfg);
    }
    return api;
})();

const cstring = (memory, ptr) => {
    if (!memory) return;
    const mem = new Uint8Array(memory.buffer, ptr);
    let len = 0;
    while (mem[len] !== 0) {
        len++;
    }
    return new TextDecoder('utf-8').decode(new Uint8Array(memory.buffer, ptr, len));
}

async function main() {
    const wasmPath = process.argv[2];
    const entryPoint = process.argv[3];
    if (!wasmPath) {
        console.error('Usage: node executer.js <path/to/test.wasm> [entry_point]');
        process.exit(1);
    }

    let memory;

    const importObject = {
        env: {
            __lea_abort: (_line) => {
                const line = Number(_line);
                print.red(`[ABORT] at line ${line}\n`);
                process.exit(1);
            },
            __lea_log: (ptr, len) => {
                if (!memory) return;
                const _len = Number(len);
                const mem = new Uint8Array(memory.buffer, ptr, _len);
                const m = new TextDecoder('utf-8').decode(mem);
                print.orange(m);
            },
            __lea_ubsen(_name, _filename, _line, _column) {
                const name = cstring(memory, _name);
                const filename = cstring(memory, _filename);
                const line = Number(_line);
                const column = Number(_column);
                print.red(`[UBSEN] ${name} at ${filename}:${line}:${column}\n`);
                process.exit(1);
            }
        },
    };

    try {
        const wasmBytes = await fs.readFile(wasmPath);
        const { instance } = await WebAssembly.instantiate(wasmBytes, importObject);
        memory = instance.exports.memory;

        const funcName = entryPoint || 'run_test';
        const func = instance.exports[funcName];

        if (typeof func !== 'function') {
            throw new Error(`'${funcName}' function not exported from ${wasmPath}`);
        }


        process.exit(func());
    } catch (e) {
        console.log(e);
        if (e instanceof WebAssembly.RuntimeError) {
            console.log(e);

            console.error(`VM error: ${e.message}`);
        } else {
            console.error(`Execution error ${wasmPath}:`);
            console.error(e);
            process.exit(1);
        }
    }
}

main().catch(e => {
    console.error(e);
    process.exit(1);
});