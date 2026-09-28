const output = document.querySelector("#logs");
const status = document.querySelector("#status");
const scrollArea = document.querySelector("main");
const logUrl = `LabProg.log?cache=${Date.now()}`;

async function refreshLogs() {
	const shouldFollow = scrollArea.scrollTop + scrollArea.clientHeight >= scrollArea.scrollHeight - 40;

	try {
		const response = await fetch(`${logUrl}&t=${Date.now()}`, { cache: "no-store" });
		if (!response.ok) {
			throw new Error(`HTTP ${response.status}`);
		}

		const text = await response.text();
		output.textContent = text || "LabProg está activo; todavía no ha escrito logs.";
		status.textContent = "Actualizando en vivo";
		status.dataset.state = "live";

		if (shouldFollow) {
			scrollArea.scrollTop = scrollArea.scrollHeight;
		}
	} catch {
		status.textContent = "Esperando a LabProg...";
		status.dataset.state = "error";
	}
}

refreshLogs();
window.setInterval(refreshLogs, 1000);
