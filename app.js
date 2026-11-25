// esptool-js is the Espressif Web Serial implementation. The project can pin
// and vendor this dependency before a production deployment; the documented
// single-file CDN bundle keeps this repository dependency-free for now.
let ESPLoader;
let Transport;

const elements = {
  browserNotice: document.querySelector("#browserNotice"),
  connect: document.querySelector("#connectButton"),
  disconnect: document.querySelector("#disconnectButton"),
  connectionState: document.querySelector("#connectionState"),
  chipInfo: document.querySelector("#chipInfo"),
  releaseVersion: document.querySelector("#releaseVersion"),
  releaseSummary: document.querySelector("#releaseSummary"),
  releaseFiles: document.querySelector("#releaseFiles"),
  flashRelease: document.querySelector("#flashReleaseButton"),
  flashManual: document.querySelector("#flashManualButton"),
  manualFiles: document.querySelector("#manualFiles"),
  eraseAll: document.querySelector("#eraseAll"),
  clearLog: document.querySelector("#clearLogButton"),
  log: document.querySelector("#log"),
  progress: document.querySelector("#progressValue"),
  progressText: document.querySelector("#progressText"),
  workflow: [...document.querySelectorAll(".step")],
};

const state = {
  transport: null,
  loader: null,
  connected: false,
  busy: false,
  release: null,
};

function log(message) {
  const timestamp = new Date().toLocaleTimeString("pt-BR", { hour12: false });
  elements.log.textContent += `\n[${timestamp}] ${String(message).trim()}`;
  elements.log.scrollTop = elements.log.scrollHeight;
}

function setConnection(connected, detail = "") {
  state.connected = connected;
  elements.connectionState.textContent = connected ? "Conectado" : "Desconectado";
  elements.connectionState.classList.toggle("connected", connected);
  elements.connect.disabled = connected || state.busy;
  elements.disconnect.disabled = !connected || state.busy;
  elements.chipInfo.textContent = detail || (connected ? "Placa conectada." : "Aguardando uma conexão.");
  updateActions();
}

function setBusy(busy, label = "") {
  state.busy = busy;
  elements.connect.disabled = busy || state.connected;
  elements.disconnect.disabled = busy || !state.connected;
  elements.flashRelease.disabled = busy || !state.connected || !state.release?.available;
  elements.flashManual.disabled = busy || !state.connected || getManualFiles().length === 0;
  if (label) elements.progressText.textContent = label;
}

function setProgress(percent, label) {
  const safePercent = Math.max(0, Math.min(100, percent));
  elements.progress.style.width = `${safePercent}%`;
  elements.progressText.textContent = label || `${safePercent.toFixed(0)}%`;
}

function setStep(stepNumber) {
  elements.workflow.forEach((step) => step.classList.toggle("is-current", Number(step.dataset.step) === stepNumber));
}

function terminal() {
  return {
    clean() {},
    writeLine(data) { log(data); },
    write(data) { log(data); },
  };
}

function parseOffset(value) {
  const normalized = value.trim().toLowerCase();
  const isHex = /^0x[\da-f]+$/.test(normalized);
  const isDecimal = /^\d+$/.test(normalized);
  if (!isHex && !isDecimal) throw new Error(`Offset inválido: ${value}`);
  const offset = Number.parseInt(normalized, isHex ? 16 : 10);
  if (!Number.isSafeInteger(offset) || offset < 0 || offset % 0x1000 !== 0) {
    throw new Error(`Offset deve ser positivo e alinhado a 0x1000: ${value}`);
  }
  return offset;
}

function getManualFiles() {
  return [...elements.manualFiles.querySelectorAll(".manual-row:not(.manual-head)")]
    .map((row) => ({
      addressInput: row.querySelector('input[type="text"]'),
      input: row.querySelector('input[type="file"]'),
      label: row.querySelector("span").textContent.trim(),
    }))
    .filter((row) => row.input.files.length > 0)
    .map((row) => ({ file: row.input.files[0], address: parseOffset(row.addressInput.value), label: row.label }));
}

function updateActions() {
  const hasManualFiles = (() => {
    try { return getManualFiles().length > 0; } catch { return false; }
  })();
  elements.flashRelease.disabled = state.busy || !state.connected || !state.release?.available;
  elements.flashManual.disabled = state.busy || !state.connected || !hasManualFiles;
}

function renderRelease(manifest) {
  elements.releaseFiles.textContent = "";
  const available = manifest?.available === true && Array.isArray(manifest.files) && manifest.files.length > 0 &&
    manifest.files.every((file) => typeof file.sha256 === "string" && /^[a-f\d]{64}$/i.test(file.sha256));
  state.release = { ...manifest, available };
  elements.releaseVersion.textContent = available ? `v${manifest.version}` : "Sem release";
  elements.releaseVersion.classList.toggle("ready", available);
  elements.releaseSummary.textContent = available
    ? manifest.notes || "Pacote verificado disponível para a T-Embed AI."
    : manifest?.reason || "Nenhum binário de release foi publicado ainda. Compile e empacote antes de gravar.";

  for (const item of manifest?.files || []) {
    const row = document.createElement("li");
    const filename = document.createElement("span");
    const offset = document.createElement("span");
    filename.textContent = item.name;
    offset.textContent = item.address;
    row.append(filename, offset);
    elements.releaseFiles.append(row);
  }
  updateActions();
}

async function loadManifest() {
  try {
    const response = await fetch("firmware/manifest.json", { cache: "no-store" });
    if (!response.ok) throw new Error(`manifesto retornou HTTP ${response.status}`);
    const manifest = await response.json();
    if (manifest.board !== "t_embed_ai" || manifest.chip !== "ESP32-S3") {
      throw new Error("manifesto não corresponde à T-Embed AI / ESP32-S3");
    }
    renderRelease(manifest);
    log(manifest.available ? `Release v${manifest.version} encontrada.` : "Manifesto carregado: nenhuma release disponível.");
  } catch (error) {
    renderRelease({ available: false, reason: "Manifesto de release indisponível. Sirva esta pasta por HTTP/HTTPS." });
    log(`Não foi possível carregar o manifesto: ${error.message}`);
  }
}

async function connect() {
  if (!navigator.serial) {
    elements.browserNotice.hidden = false;
    return;
  }
  try {
    setBusy(true, "Conectando");
    log("Solicitando porta serial…");
    const port = await navigator.serial.requestPort();
    const transport = new Transport(port, true);
    const loader = new ESPLoader({
      transport,
      baudrate: 115200,
      terminal: terminal(),
      serialOptions: { bufferSize: 65536 },
    });
    const chip = await loader.main();
    if (!/esp32-s3/i.test(chip)) {
      await transport.disconnect();
      throw new Error(`Placa incompatível: detectada ${chip}; esperado ESP32-S3.`);
    }
    state.transport = transport;
    state.loader = loader;
    setConnection(true, `Detectado: ${chip} · ROM a 115200 baud.`);
    setStep(2);
    log(`Conectado a ${chip}.`);
  } catch (error) {
    setConnection(false, "Não foi possível conectar à placa.");
    log(`Erro de conexão: ${error.message || error}`);
  } finally {
    setBusy(false, state.connected ? "Conectado" : "Pronto");
  }
}

async function disconnect() {
  try {
    if (state.transport) await state.transport.disconnect();
    log("Porta serial desconectada.");
  } catch (error) {
    log(`Aviso ao desconectar: ${error.message || error}`);
  } finally {
    state.transport = null;
    state.loader = null;
    setConnection(false);
    setStep(1);
  }
}

async function firmwareFileFromUrl(item) {
  const response = await fetch(`firmware/${item.path}`, { cache: "no-store" });
  if (!response.ok) throw new Error(`Não foi possível baixar ${item.name} (HTTP ${response.status}).`);
  const buffer = await response.arrayBuffer();
  const digest = await crypto.subtle.digest("SHA-256", buffer);
  const actualHash = [...new Uint8Array(digest)].map((byte) => byte.toString(16).padStart(2, "0")).join("");
  if (actualHash.toLowerCase() !== item.sha256.toLowerCase()) {
    throw new Error(`Checksum inválido para ${item.name}. A imagem não será gravada.`);
  }
  return { data: new Uint8Array(buffer), address: parseOffset(item.address), label: item.name };
}

async function flash(files, eraseAll) {
  if (!state.connected || !state.loader) throw new Error("Conecte uma T-Embed ESP32-S3 antes de gravar.");
  if (files.length === 0) throw new Error("Selecione ao menos um arquivo de firmware.");
  const addresses = new Set();
  for (const item of files) {
    if (addresses.has(item.address)) throw new Error(`Há dois arquivos no offset 0x${item.address.toString(16)}.`);
    addresses.add(item.address);
  }

  const totalBytes = files.reduce((sum, item) => sum + item.data.length, 0);
  const starts = files.reduce((accumulator, item, index) => {
    accumulator[index] = index === 0 ? 0 : accumulator[index - 1] + files[index - 1].data.length;
    return accumulator;
  }, []);

  setBusy(true, "Preparando");
  setStep(3);
  setProgress(0, "0%");
  log(`Iniciando gravação de ${files.length} arquivo(s), ${Math.ceil(totalBytes / 1024)} KB.`);
  try {
    await state.loader.writeFlash({
      fileArray: files.map(({ data, address }) => ({ data, address })),
      flashSize: "16MB",
      flashMode: "qio",
      flashFreq: "80m",
      eraseAll,
      compress: true,
      reportProgress(fileIndex, written, total) {
        const progress = ((starts[fileIndex] + written) / totalBytes) * 100;
        setProgress(progress, `${progress.toFixed(0)}%`);
        if (written === total) log(`${files[fileIndex].label || `arquivo ${fileIndex + 1}`} gravado.`);
      },
    });
    setProgress(100, "100%");
    log("Verificação da gravação concluída pelo esptool-js.");
    await state.loader.after("hard_reset");
    log("Reset aplicado. Aguarde a inicialização da T-Embed AI.");
  } catch (error) {
    log(`Falha na gravação: ${error.message || error}`);
    throw error;
  } finally {
    setBusy(false, "Concluído");
  }
}

elements.connect.addEventListener("click", connect);
elements.disconnect.addEventListener("click", disconnect);
elements.clearLog.addEventListener("click", () => { elements.log.textContent = "Registro limpo."; });
elements.manualFiles.addEventListener("change", updateActions);
elements.manualFiles.addEventListener("input", updateActions);
elements.flashRelease.addEventListener("click", async () => {
  try {
    const files = await Promise.all(state.release.files.map(firmwareFileFromUrl));
    await flash(files, false);
  } catch (error) {
    log(`Imagem oficial não gravada: ${error.message || error}`);
  }
});
elements.flashManual.addEventListener("click", async () => {
  try {
    const files = await Promise.all(getManualFiles().map(async (item) => ({
      data: new Uint8Array(await item.file.arrayBuffer()), address: item.address, label: item.file.name,
    })));
    await flash(files, elements.eraseAll.checked);
  } catch (error) {
    log(`Arquivos locais não gravados: ${error.message || error}`);
  }
});

elements.connect.disabled = true;
if (!navigator.serial || !window.isSecureContext) {
  elements.browserNotice.hidden = false;
  log("Web Serial requer Chrome/Edge em HTTPS ou localhost.");
}

try {
  const module = await import("https://unpkg.com/esptool-js/bundle.js");
  ESPLoader = module.ESPLoader;
  Transport = module.Transport;
  if (!ESPLoader || !Transport) throw new Error("bundle não exportou ESPLoader e Transport");
  log("Biblioteca de gravação carregada.");
  if (navigator.serial && window.isSecureContext) elements.connect.disabled = false;
  await loadManifest();
} catch (error) {
  elements.browserNotice.hidden = false;
  elements.browserNotice.textContent = `Ferramenta de gravação indisponível. Não foi possível carregar esptool-js: ${String(error.message || error)}.`;
  elements.connect.disabled = true;
  log(`Falha ao carregar esptool-js: ${error.message || error}`);
}
