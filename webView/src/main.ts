import { CANVAS_HEIGHT, CANVAS_ID, CANVAS_WIDTH, WASM_NAME } from "./constants.js";

interface WasmInterface{
    init_scene : (width: number, height: number) => void;
    get_color_buffer: () => number;
};

function getTexture(): Uint8ClampedArray{
    const arrayBuffer = new ArrayBuffer(CANVAS_WIDTH * CANVAS_HEIGHT * 4);
    const pixels = new Uint8ClampedArray(arrayBuffer);
    for (let y = 0; y < CANVAS_HEIGHT; y++) {
        for (let x = 0; x < CANVAS_WIDTH; x++) {
            const i = (y*CANVAS_WIDTH + x) * 4;
            pixels[i  ] = 255 * x / CANVAS_WIDTH;   // red
            pixels[i+1] = 255 * y / CANVAS_HEIGHT;   // green
            pixels[i+2] = 0;   // blue
            pixels[i+3] = 255; // alpha
        }
    }
    return pixels;
}

let __memOffset = 0;
function malloc(size: number, memory: WebAssembly.Memory): number{
    while(memory.buffer.byteLength - __memOffset < size){
        /* REGROW ARRAY BUFFER, INCREASE NUMBER OF PAGES BY 1 */
        memory.grow(1);
    }
    const current = __memOffset;
    __memOffset += size;
    return current;
}

async function main(){
    console.log("SCENE TRACE");

    const canvas = document.getElementById(CANVAS_ID) as HTMLCanvasElement;
    canvas.setAttribute('width', CANVAS_WIDTH.toString());
    canvas.setAttribute('height', CANVAS_HEIGHT.toString());
    const context = canvas.getContext('2d') as CanvasRenderingContext2D;

    /* SETUP WASM MODULE */
    const memory: WebAssembly.Memory = new WebAssembly.Memory({ initial: 10 });
    const importObject = {env: {
        malloc: (value: number) => malloc(value, memory),
        sqrtf : (value: number) => Math.sqrt(value),
        fabs  : (value: number) => Math.abs(value),
        memory: memory}};
    const bytes = await (await fetch(WASM_NAME)).arrayBuffer();
    const module = await WebAssembly.instantiate(bytes, importObject);

    __memOffset= module.instance.exports.__heap_base as unknown as number;
    const wasm_api = module.instance.exports as unknown as WasmInterface;

    console.log("INITIALIZED WASM MODULE");

    wasm_api.init_scene(CANVAS_WIDTH, CANVAS_HEIGHT);
    const pixelBuffer = new Uint8ClampedArray(memory.buffer, wasm_api.get_color_buffer(), CANVAS_WIDTH * CANVAS_HEIGHT * 4);
    for(let i = 0; i < CANVAS_WIDTH * CANVAS_HEIGHT; i++){
        let tmp = pixelBuffer[4 * i]; pixelBuffer[4 * i] = pixelBuffer[4 * i + 3];
        pixelBuffer[4 * i + 3] = tmp;
        tmp = pixelBuffer[4 * i + 1]; pixelBuffer[4 * i + 1] = pixelBuffer[4 * i + 2];
        pixelBuffer[4 * i + 2] = tmp;
    }

    const imageData = new ImageData(pixelBuffer, CANVAS_WIDTH, CANVAS_HEIGHT);
    context.putImageData(imageData, 0, 0);
}

main();