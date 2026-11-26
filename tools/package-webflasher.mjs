import { createHash } from "node:crypto";
import { copyFile, mkdir, readFile, writeFile } from "node:fs/promises";
import { existsSync } from "node:fs";
import { resolve, join, dirname } from "node:path";
import { fileURLToPath } from "node:url";

function argument(name) {
  const index = process.argv.indexOf(name);
  return index === -1 ? undefined : process.argv[index + 1];
}

function help(message) {
  if (message) console.error(`Erro: ${message}\n`);
  console.error("Uso: node tools/package-webflasher.mjs --version <x.y.z> --boot-app0 <caminho>");
  process.exitCode = 1;
}

const version = argument("--version");
const bootApp0 = argument("--boot-app0");
if (!version || !/^[0-9]+\.[0-9]+\.[0-9]+(?:[-+][\w.-]+)?$/.test(version)) {
  help("informe uma versão semântica, por exemplo 0.1.0");
} else if (!bootApp0) {
  help("informe o caminho do boot_app0.bin");
} else {
  const root = resolve(dirname(fileURLToPath(import.meta.url)), "..");
  const build = join(root, ".pio", "build", "t_embed_ai");
  const releaseRelative = join("releases", version).replaceAll("\\", "/");
  const releaseDirectory = join(root, "webflasher", "firmware", releaseRelative);
  const sourceFiles = [
    { name: "bootloader.bin", source: join(build, "bootloader.bin"), output: "bootloader.bin", address: "0x0000" },
    { name: "partitions.bin", source: join(build, "partitions.bin"), output: "partitions.bin", address: "0x8000" },
    { name: "boot_app0.bin", source: resolve(bootApp0), output: "boot_app0.bin", address: "0xe000" },
    { name: `t-embed-ai-${version}.bin`, source: join(build, "firmware.bin"), output: `t-embed-ai-${version}.bin`, address: "0x10000" },
  ];

  const absent = sourceFiles.filter((file) => !existsSync(file.source)).map((file) => file.source);
  if (absent.length > 0) {
    help(`artefatos ausentes:\n${absent.map((file) => `- ${file}`).join("\n")}`);
  } else {
    await mkdir(releaseDirectory, { recursive: true });
    const manifestFiles = [];
    for (const file of sourceFiles) {
      const destination = join(releaseDirectory, file.output);
      await copyFile(file.source, destination);
      const checksum = createHash("sha256").update(await readFile(destination)).digest("hex");
      manifestFiles.push({
        name: file.name,
        path: `${releaseRelative}/${file.output}`,
        address: file.address,
        sha256: checksum,
      });
    }
    const manifest = {
      schema: 1,
      board: "t_embed_ai",
      chip: "ESP32-S3",
      available: true,
      version,
      notes: `T-Embed AI ${version}. Gerado pelo empacotador local; teste a imagem na placa antes de publicar.`,
      files: manifestFiles,
    };
    await writeFile(join(root, "webflasher", "firmware", "manifest.json"), `${JSON.stringify(manifest, null, 2)}\n`);
    console.log(`Release v${version} publicada em webflasher/firmware/${releaseRelative}.`);
  }
}
