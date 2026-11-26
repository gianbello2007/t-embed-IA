import { existsSync, readFileSync } from "node:fs";

const required = [
  "platformio.ini",
  "boards/t_embed_ai.json",
  "partitions.csv",
  "include/BoardPins.h",
  "src/main.cpp",
  "src/core/System.cpp",
  "docs/HARDWARE_AUDIT.md",
  "docs/ARCHITECTURE.md",
  "docs/BUILD.md",
];

const absent = required.filter((file) => !existsSync(file));
if (absent.length > 0) {
  throw new Error(`Missing required files: ${absent.join(", ")}`);
}

const pins = readFileSync("include/BoardPins.h", "utf8");
const expected = [
  "kSharedPower = GPIO_NUM_15",
  "kI2cSda = GPIO_NUM_8",
  "kI2cScl = GPIO_NUM_18",
  "kDisplayCs = GPIO_NUM_41",
  "kSdCs = GPIO_NUM_13",
  "kCc1101Cs = GPIO_NUM_12",
];
for (const value of expected) {
  if (!pins.includes(value)) {
    throw new Error(`Board map does not include ${value}`);
  }
}

const config = readFileSync("platformio.ini", "utf8");
for (const value of ["board = t_embed_ai", "memory_type = qio_opi", "ARDUINO_USB_CDC_ON_BOOT=1"]) {
  if (!config.includes(value)) {
    throw new Error(`Build configuration does not include ${value}`);
  }
}

console.log("Project contract checks passed.");
