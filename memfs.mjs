import fs, { readFileSync } from 'fs';

const file = fs.readFileSync('memfs.wasm');
const compiled = new WebAssembly.Module(file);

export function newMemfs(getExternalMemory) {
  const memory = new WebAssembly.Memory({initial: 16});
  const u8 = new Uint8Array(memory.buffer);
  function err_unimplemented() {
    throw 'unimplemented';
  }
  const imports = {
    env: {
      memory,
      console_log(ptr) {
        let str = '';
        while (u8[ptr] != 0) {
          str += String.fromCharCode(u8[ptr]);
          ptr++;
        }
        console.log(str);
      },
      read_in(dest, extern_src, size) {
        const external = getExternalMemory();
        const exU8 = new Uint8Array(external.buffer);
        for (let i = 0; i < size; ++i) {
          u8[dest + i] = exU8[extern_src + i];
        }
      },
      write_out(extern_dest, src, size) {
        const external = getExternalMemory();
        const exU8 = new Uint8Array(external.buffer);
        for (let i = 0; i < size; ++i) {
          exU8[extern_dest + i] = u8[src + i];
        }
      },
    },
    wasi_unstable: {
      fd_close: err_unimplemented,
      fd_write: err_unimplemented,
      fd_seek: err_unimplemented,
    },
  };
  const instance = new WebAssembly.Instance(compiled, imports);
  return instance.exports;
};
