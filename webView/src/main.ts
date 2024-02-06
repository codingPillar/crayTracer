import { CANVAS_HEIGHT, CANVAS_ID, CANVAS_WIDTH } from "./constants.js";

function main(){
    console.log("SCENE TRACE");

    const canvas = document.getElementById(CANVAS_ID) as HTMLCanvasElement;
    canvas.setAttribute('width', CANVAS_WIDTH.toString());
    canvas.setAttribute('height', CANVAS_HEIGHT.toString());
    const context = canvas.getContext('2d') as CanvasRenderingContext2D;

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

    const imageData = new ImageData(pixels, CANVAS_WIDTH, CANVAS_HEIGHT);
    context.putImageData(imageData, 0, 0);
}

main();