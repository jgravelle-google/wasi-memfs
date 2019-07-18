import fs, { readFileSync } from 'fs';
import { newMemfs } from './memfs.mjs';
// import * as wasi from './wasi.mjs';

const memory = new WebAssembly.Memory({initial: 16});
let memfs = newMemfs(() => memory);

const file = fs.readFileSync(process.argv[2]);
const compiled = new WebAssembly.Module(file);
const imports = {
  env: { memory },
  wasi_unstable: memfs,
};
const instance = new WebAssembly.Instance(compiled, imports);

instance.exports.main();
