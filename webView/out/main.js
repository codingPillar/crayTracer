var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    function adopt(value) { return value instanceof P ? value : new P(function (resolve) { resolve(value); }); }
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : adopt(result.value).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
import { CANVAS_HEIGHT, CANVAS_ID, CANVAS_WIDTH, WASM_NAME } from "./constants.js";
;
function getTexture() {
    const arrayBuffer = new ArrayBuffer(CANVAS_WIDTH * CANVAS_HEIGHT * 4);
    const pixels = new Uint8ClampedArray(arrayBuffer);
    for (let y = 0; y < CANVAS_HEIGHT; y++) {
        for (let x = 0; x < CANVAS_WIDTH; x++) {
            const i = (y * CANVAS_WIDTH + x) * 4;
            pixels[i] = 255 * x / CANVAS_WIDTH; // red
            pixels[i + 1] = 255 * y / CANVAS_HEIGHT; // green
            pixels[i + 2] = 0; // blue
            pixels[i + 3] = 255; // alpha
        }
    }
    return pixels;
}
let __memOffset = 0;
function malloc(size, memory) {
    while (memory.buffer.byteLength - __memOffset < size) {
        /* REGROW ARRAY BUFFER, INCREASE NUMBER OF PAGES BY 1 */
        memory.grow(1);
    }
    const current = __memOffset;
    __memOffset += size;
    return current;
}
function main() {
    return __awaiter(this, void 0, void 0, function* () {
        console.log("SCENE TRACE");
        const canvas = document.getElementById(CANVAS_ID);
        canvas.setAttribute('width', CANVAS_WIDTH.toString());
        canvas.setAttribute('height', CANVAS_HEIGHT.toString());
        const context = canvas.getContext('2d');
        /* SETUP WASM MODULE */
        const memory = new WebAssembly.Memory({ initial: 10 });
        const importObject = { env: {
                malloc: (value) => malloc(value, memory),
                sqrtf: (value) => Math.sqrt(value),
                fabs: (value) => Math.abs(value),
                memory: memory
            } };
        const bytes = yield (yield fetch(WASM_NAME)).arrayBuffer();
        const module = yield WebAssembly.instantiate(bytes, importObject);
        __memOffset = module.instance.exports.__heap_base;
        const wasm_api = module.instance.exports;
        console.log("INITIALIZED WASM MODULE");
        wasm_api.init_scene(CANVAS_WIDTH, CANVAS_HEIGHT);
        const pixelBuffer = new Uint8ClampedArray(memory.buffer, wasm_api.get_color_buffer(), CANVAS_WIDTH * CANVAS_HEIGHT * 4);
        for (let i = 0; i < CANVAS_WIDTH * CANVAS_HEIGHT; i++) {
            let tmp = pixelBuffer[4 * i];
            pixelBuffer[4 * i] = pixelBuffer[4 * i + 3];
            pixelBuffer[4 * i + 3] = tmp;
            tmp = pixelBuffer[4 * i + 1];
            pixelBuffer[4 * i + 1] = pixelBuffer[4 * i + 2];
            pixelBuffer[4 * i + 2] = tmp;
        }
        const imageData = new ImageData(pixelBuffer, CANVAS_WIDTH, CANVAS_HEIGHT);
        context.putImageData(imageData, 0, 0);
    });
}
main();
