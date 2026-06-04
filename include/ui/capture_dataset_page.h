#pragma once

static const char CAPTURE_DATASET_HTML[] = R"rawliteral(
<!DOCTYPE html>
<html lang="pl">
<head>
  <meta charset="UTF-8">
  <title>Zbieranie zdjęć — ESP32-CAM → Teachable Machine</title>
  <script src="https://cdnjs.cloudflare.com/ajax/libs/jszip/3.10.1/jszip.min.js"></script>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; }
    body {
      background: #1a1a1a; color: #eee;
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", sans-serif;
      padding: 20px;
    }
    h1 { color: #e74c3c; margin-bottom: 6px; font-size: 22px; }
    .subtitle { color: #888; font-size: 13px; margin-bottom: 20px; }

    .warning {
      background: #4a2c1a; border-left: 4px solid #e67e22;
      padding: 10px 14px; border-radius: 4px;
      font-size: 13px; color: #f5c891; margin-bottom: 16px;
      display: none;
    }
    .warning.show { display: block; }

    .setup {
      background: #2a2a2a; padding: 16px; border-radius: 8px;
      margin-bottom: 20px; display: flex; gap: 10px;
      align-items: center; flex-wrap: wrap;
    }
    .setup label { font-size: 14px; color: #bbb; }
    .setup input, .setup select {
      padding: 8px 12px; border-radius: 6px; border: none;
      font-size: 14px; background: #3a3a3a; color: #eee;
    }
    .setup input { width: 160px; }
    .setup button {
      padding: 8px 16px; border-radius: 6px; border: none;
      background: #e74c3c; color: #fff; font-size: 14px; cursor: pointer;
    }
    .setup button:hover { background: #c0392b; }
    .setup button.secondary { background: #555; }
    .setup button.secondary:hover { background: #666; }

    .main { display: flex; gap: 20px; flex-wrap: wrap; }

    .camera-box {
      background: #2a2a2a; border-radius: 8px; padding: 16px;
      flex: 0 0 auto; display: flex; flex-direction: column; gap: 10px;
    }
    .camera-box img {
      display: block; width: 320px; height: 240px;
      background: #000; border-radius: 6px; object-fit: cover;
    }
    .stream-info {
      font-size: 12px; color: #888;
      display: flex; justify-content: space-between;
    }
    .stream-info .fps { color: #2ecc71; }
    canvas { display: none; }

    .class-panel { background: #2a2a2a; border-radius: 8px; padding: 16px; flex: 1 1 320px; }
    .class-panel h2 { margin-bottom: 14px; font-size: 16px; color: #aaa; }

    .class-row { display: flex; gap: 10px; align-items: center; margin-bottom: 12px; }
    .class-row input {
      flex: 1; padding: 8px 12px; border-radius: 6px; border: none;
      font-size: 14px; background: #3a3a3a; color: #eee;
    }
    .class-row button.add {
      padding: 8px 14px; border-radius: 6px; border: none;
      background: #27ae60; color: #fff; cursor: pointer; font-size: 14px;
    }
    .class-row button.add:hover { background: #1e8449; }

    .classes { display: flex; flex-direction: column; gap: 12px; }
    .class-item { background: #333; border-radius: 8px; padding: 12px; }
    .class-header { display: flex; align-items: center; gap: 10px; margin-bottom: 8px; flex-wrap: wrap; }
    .class-header .name { font-weight: 600; flex: 1; min-width: 80px; }
    .class-header .count {
      color: #aaa; font-size: 13px;
      background: #1a1a1a; padding: 2px 8px; border-radius: 10px;
    }
    .class-header .count.active { background: #27ae60; color: #fff; }
    .class-header button.capture {
      padding: 6px 12px; border-radius: 6px; border: none;
      background: #2980b9; color: #fff; cursor: pointer; font-size: 13px;
    }
    .class-header button.capture:hover { background: #1a6a9a; }
    .class-header button.capture.recording {
      background: #e74c3c; animation: pulse 1s infinite;
    }
    @keyframes pulse {
      0%, 100% { opacity: 1; } 50% { opacity: 0.6; }
    }
    .class-header button.del {
      padding: 6px 10px; border-radius: 6px; border: none;
      background: #555; color: #fff; cursor: pointer; font-size: 13px;
    }
    .class-header button.del:hover { background: #e74c3c; }

    .thumbs {
      display: flex; flex-wrap: wrap; gap: 5px;
      max-height: 110px; overflow-y: auto;
    }
    .thumbs img {
      width: 56px; height: 42px; object-fit: cover;
      border-radius: 4px; border: 2px solid #555;
    }
    .thumbs::-webkit-scrollbar { width: 6px; }
    .thumbs::-webkit-scrollbar-thumb { background: #555; border-radius: 3px; }

    .actions { margin-top: 18px; display: flex; gap: 10px; flex-wrap: wrap; }
    .actions button {
      padding: 10px 20px; border-radius: 8px; border: none;
      font-size: 14px; cursor: pointer; font-weight: 500;
    }
    .btn-zip { background: #8e44ad; color: #fff; }
    .btn-zip:hover { background: #6c3483; }
    .btn-tm { background: #e67e22; color: #fff; }
    .btn-tm:hover { background: #b9770e; }
    .btn-clear { background: #555; color: #fff; }
    .btn-clear:hover { background: #444; }

    .status {
      margin-top: 14px; color: #2ecc71; font-size: 13px;
      min-height: 18px; font-family: monospace;
    }
    .error { color: #e74c3c; }

    .tip {
      background: #2a2a2a; border-radius: 8px; padding: 14px;
      margin-top: 20px; font-size: 13px; color: #aaa; line-height: 1.7;
    }
    .tip strong { color: #eee; }
    .tip kbd {
      background: #444; padding: 2px 6px; border-radius: 4px;
      font-size: 11px; border: 1px solid #555;
    }
    .tip details { margin-top: 8px; }
    .tip summary { cursor: pointer; color: #ccc; }
    .tip summary:hover { color: #fff; }
  </style>
</head>
<body>
  <h1>ESP32-CAM → Teachable Machine</h1>
  <div class="subtitle">Zbieranie i etykietowanie zdjęć do treningu modelu</div>

  <div id="httpsWarn" class="warning">
    ⚠ Strona załadowana przez HTTPS — przeglądarka zablokuje połączenie HTTP do ESP32.
    Otwórz ten plik lokalnie (file://) lub przez HTTP, żeby kamera działała.
  </div>

  <div class="setup">
    <label>IP ESP32:</label>
    <input id="ip" type="text" value="192.168.4.1" placeholder="np. 192.168.4.1">

    <label>Tryb:</label>
    <select id="mode">
      <option value="stream">MJPEG stream (płynny)</option>
      <option value="poll">Polling /capture (kompatybilny)</option>
    </select>

    <label>Port stream:</label>
    <input id="streamPort" type="text" value="81" style="width:60px;">

    <button onclick="connectCamera()">Połącz</button>
    <button class="secondary" onclick="disconnectCamera()">Rozłącz</button>
    <span id="connStatus" style="color:#aaa; font-size:13px; margin-left:6px;"></span>
  </div>

  <div class="main">
    <div class="camera-box">
      <img id="stream" src="" alt="Podgląd kamery" crossorigin="anonymous">
      <div class="stream-info">
        <span id="modeLabel">—</span>
        <span class="fps" id="fpsLabel"></span>
      </div>
    </div>
    <canvas id="canvas"></canvas>

    <div class="class-panel">
      <h2>Klasy</h2>
      <div class="class-row">
        <input id="newClassName" type="text"
               placeholder="Nazwa klasy (np. przód, stop…)"
               onkeydown="if(event.key==='Enter') addClass()">
        <button class="add" onclick="addClass()">+ Dodaj</button>
      </div>
      <div class="classes" id="classList"></div>

      <div class="actions">
        <button class="btn-zip" onclick="downloadZip()">⬇ Pobierz ZIP</button>
        <button class="btn-tm" onclick="openTM()">Otwórz Teachable Machine</button>
        <button class="btn-clear" onclick="clearAll()">🗑 Wyczyść wszystko</button>
      </div>
      <div class="status" id="status"></div>
    </div>
  </div>

  <div class="tip">
    <strong>Skróty klawiszowe:</strong>
    <kbd>1</kbd>–<kbd>9</kbd> — zrób zdjęcie do klasy nr 1–9 ·
    <kbd>Spacja</kbd> — przytrzymaj, żeby nagrywać serię (10 fps) do aktywnej klasy ·
    Kliknięcie na klasę = ustawia aktywną.

    <details>
      <summary><strong>Wskazówki techniczne</strong></summary>
      <ul style="margin-top:8px; padding-left:20px;">
        <li><strong>MJPEG stream:</strong> standardowy firmware ESP32-CAM (CameraWebServer.ino) udostępnia stream pod portem 81. Jeśli nie działa, przełącz na "Polling".</li>
        <li><strong>Polling:</strong> odpytuje <code>/capture</code> co 200 ms — działa wszędzie ale obciąża ESP32.</li>
        <li><strong>HTTPS:</strong> jeśli widzisz ostrzeżenie u góry — zapisz tę stronę lokalnie i otwórz przez file:// (mixed content blokuje HTTP do ESP32).</li>
        <li><strong>Rozdzielczość:</strong> Teachable Machine i tak zmniejsza do 224×224, więc QVGA 320×240 z ESP32 wystarczy.</li>
        <li><strong>Format ZIP:</strong> Teachable Machine nie wczytuje folderów ZIP bezpośrednio — w TM trzeba ładować zdjęcia per klasa ("Upload"). ZIP jest do archiwizacji lub treningu we własnym skrypcie (Keras/TensorFlow).</li>
        <li><strong>Min. 50–100 zdjęć</strong> na klasę dla rozsądnego modelu. Zmieniaj kąt, oświetlenie, tło.</li>
      </ul>
    </details>
  </div>

  <script>
    // ---------- State ----------
    const classes = {};        // { name: [dataURL, ...] }
    let activeClass = null;
    let pollTimer = null;
    let frameCount = 0;
    let fpsTimer = null;
    let mode = 'stream';

    // Auto-fill IP if hosted on ESP32
    if (window.location.hostname && window.location.hostname !== 'localhost' && window.location.hostname !== '') {
      document.getElementById('ip').value = window.location.hostname;
      document.getElementById('mode').value = 'poll'; // Ustawiamy poll domyślnie, jest stabilniejszy i nie ma problemu CORS
    }

    // ---------- HTTPS warning ----------
    if (location.protocol === 'https:') {
      document.getElementById('httpsWarn').classList.add('show');
    }

    // ---------- Camera connection ----------
    function connectCamera() {
      disconnectCamera();
      const ip = document.getElementById('ip').value.trim();
      mode = document.getElementById('mode').value;
      const port = document.getElementById('streamPort').value.trim() || '81';
      const img = document.getElementById('stream');
      const cs = document.getElementById('connStatus');
      const ml = document.getElementById('modeLabel');

      cs.textContent = 'Łączenie…'; cs.style.color = '#aaa';

      // Test connection with /capture first (works for both modes)
      const test = new Image();
      test.crossOrigin = "anonymous";
      test.onload = () => {
        cs.textContent = '✓ Połączono'; cs.style.color = '#2ecc71';
        startStream(ip, port);
        startFpsCounter();
      };
      test.onerror = () => {
        cs.textContent = '✗ Błąd — sprawdź IP i WiFi';
        cs.style.color = '#e74c3c';
        ml.textContent = '—';
      };
      test.src = `http://${ip}/capture?t=${Date.now()}`;
    }

    function startStream(ip, port) {
      const img = document.getElementById('stream');
      const ml = document.getElementById('modeLabel');

      if (mode === 'stream') {
        // MJPEG stream from port 81
        img.src = `http://${ip}:${port}/stream`;
        ml.textContent = `MJPEG stream :${port}`;
      } else {
        // Polling mode — refresh sequentially to not crash ESP
        ml.textContent = 'Polling /capture';
        const refresh = () => {
          if (mode !== 'poll') return;
          const tmp = new Image();
          tmp.crossOrigin = "anonymous";
          tmp.onload = () => {
            img.src = tmp.src;
            frameCount++;
            if (pollTimer) pollTimer = setTimeout(refresh, 200);
          };
          tmp.onerror = () => {
            if (pollTimer) pollTimer = setTimeout(refresh, 1000);
          };
          tmp.src = `http://${ip}/capture?t=${Date.now()}`;
        };
        pollTimer = setTimeout(refresh, 0);
      }
    }

    function startFpsCounter() {
      const fpsEl = document.getElementById('fpsLabel');
      let last = frameCount;
      fpsTimer = setInterval(() => {
        if (mode === 'poll') {
          const fps = frameCount - last;
          last = frameCount;
          fpsEl.textContent = fps + ' fps';
        } else {
          fpsEl.textContent = '● live';
        }
      }, 1000);
    }

    function disconnectCamera() {
      if (pollTimer) { clearTimeout(pollTimer); pollTimer = null; }
      if (fpsTimer) { clearInterval(fpsTimer); fpsTimer = null; }
      document.getElementById('stream').src = '';
      document.getElementById('connStatus').textContent = '';
      document.getElementById('modeLabel').textContent = '—';
      document.getElementById('fpsLabel').textContent = '';
      frameCount = 0;
    }

    // ---------- Class management ----------
    function addClass() {
      const input = document.getElementById('newClassName');
      const name = input.value.trim();
      if (!name) return;
      if (classes[name] !== undefined) {
        setStatus('Klasa już istnieje!', true);
        return;
      }
      classes[name] = [];
      if (!activeClass) activeClass = name;
      input.value = '';
      renderClasses();
    }

    function removeClass(name) {
      if (!confirm(`Usunąć klasę "${name}" i ${classes[name].length} zdjęć?`)) return;
      delete classes[name];
      if (activeClass === name) {
        const remaining = Object.keys(classes);
        activeClass = remaining[0] || null;
      }
      renderClasses();
    }

    function setActive(name) {
      activeClass = name;
      renderClasses();
    }

    async function captureImage(name) {
      const img = document.getElementById('stream');
      const canvas = document.getElementById('canvas');
      const w = img.naturalWidth || img.width || 320;
      const h = img.naturalHeight || img.height || 240;
      
      if (!img.src || img.src.length < 10) {
        setStatus('Brak obrazu z kamery!', true);
        return;
      }

      canvas.width = w;
      canvas.height = h;
      const ctx = canvas.getContext('2d');

      try {
        ctx.drawImage(img, 0, 0, w, h);
        const dataURL = canvas.toDataURL('image/jpeg', 0.92);
        classes[name].push(dataURL);
        renderClasses();
        setStatus(`[${name}] ${classes[name].length} zdjęć`);
      } catch (e) {
        console.error('Canvas error:', e);
        if (mode === 'stream' && e.name === 'SecurityError') {
           setStatus('Blokada CORS (przeglądarka zablokowała zdjęcie). Przełącz Tryb na "Polling /capture" u góry i kliknij Połącz!', true);
        } else {
           setStatus('Błąd przy robieniu zdjęcia: ' + e.message, true);
        }
      }
    }

    function sanitize(name) {
      return name.replace(/[^a-zA-Z0-9_-]/g, '_');
    }

    function renderClasses() {
      const list = document.getElementById('classList');
      list.innerHTML = '';
      const names = Object.keys(classes);
      names.forEach((name, idx) => {
        const images = classes[name];
        const div = document.createElement('div');
        div.className = 'class-item';
        if (name === activeClass) {
          div.style.outline = '2px solid #27ae60';
        }
        const sid = sanitize(name);
        const shortcut = idx < 9 ? `<span style="color:#888; font-size:11px; margin-right:6px;">[${idx+1}]</span>` : '';
        div.innerHTML = `
          <div class="class-header">
            ${shortcut}
            <span class="name">${escapeHtml(name)}</span>
            <span class="count ${name === activeClass ? 'active' : ''}">${images.length}</span>
            <button class="capture" onclick="event.stopPropagation(); captureImage('${escapeAttr(name)}')">📷</button>
            <button class="del" onclick="event.stopPropagation(); removeClass('${escapeAttr(name)}')">✕</button>
          </div>
          <div class="thumbs" id="thumbs-${sid}"></div>
        `;
        div.onclick = () => setActive(name);
        list.appendChild(div);
        const thumbs = document.getElementById('thumbs-' + sid);
        // Show last 30 thumbs to avoid DOM bloat
        const start = Math.max(0, images.length - 30);
        for (let i = start; i < images.length; i++) {
          const t = document.createElement('img');
          t.src = images[i];
          thumbs.appendChild(t);
        }
        thumbs.scrollLeft = thumbs.scrollWidth;
      });
    }

    function escapeHtml(s) {
      return s.replace(/[&<>"']/g, c => ({'&':'&amp;','<':'&lt;','>':'&gt;','"':'&quot;',"'":'&#39;'}[c]));
    }
    function escapeAttr(s) {
      return s.replace(/'/g, "\\'").replace(/"/g, '\\"');
    }

    // ---------- Keyboard shortcuts ----------
    let spaceHeld = false;
    let burstTimer = null;

    document.addEventListener('keydown', (e) => {
      // Don't trigger when typing in inputs
      if (e.target.tagName === 'INPUT' || e.target.tagName === 'SELECT') return;

      // Number keys 1-9 → capture to that class
      if (/^[1-9]$/.test(e.key)) {
        const idx = parseInt(e.key, 10) - 1;
        const names = Object.keys(classes);
        if (names[idx]) {
          captureImage(names[idx]);
          setActive(names[idx]);
        }
        return;
      }

      // Space → burst capture to active class
      if (e.code === 'Space' && !spaceHeld && activeClass) {
        e.preventDefault();
        spaceHeld = true;
        burstTimer = setInterval(() => {
          captureImage(activeClass);
        }, 100); // 10 fps
      }
    });

    document.addEventListener('keyup', (e) => {
      if (e.code === 'Space' && spaceHeld) {
        spaceHeld = false;
        if (burstTimer) { clearInterval(burstTimer); burstTimer = null; }
      }
    });

    // ---------- Export ----------
    async function downloadZip() {
      const total = Object.values(classes).reduce((s, a) => s + a.length, 0);
      if (total === 0) { setStatus('Brak zdjęć do pobrania!', true); return; }
      setStatus('Pakowanie ZIP…');
      const zip = new JSZip();
      for (const [name, images] of Object.entries(classes)) {
        const folder = zip.folder(sanitize(name));
        images.forEach((dataURL, i) => {
          folder.file(
            `${sanitize(name)}_${String(i + 1).padStart(4, '0')}.jpg`,
            dataURL.split(',')[1],
            { base64: true }
          );
        });
      }
      const blob = await zip.generateAsync({ type: 'blob' });
      const a = document.createElement('a');
      a.href = URL.createObjectURL(blob);
      a.download = `dataset_esp32_${Date.now()}.zip`;
      a.click();
      setStatus(`✓ ZIP: ${total} zdjęć w ${Object.keys(classes).length} klasach`);
    }

    function openTM() {
      window.open('https://teachablemachine.withgoogle.com/train/image', '_blank');
    }

    function clearAll() {
      const total = Object.values(classes).reduce((s, a) => s + a.length, 0);
      if (total === 0) {
        for (const k of Object.keys(classes)) delete classes[k];
        activeClass = null;
        renderClasses();
        return;
      }
      if (!confirm(`Usunąć wszystkie klasy i ${total} zdjęć?`)) return;
      for (const k of Object.keys(classes)) delete classes[k];
      activeClass = null;
      renderClasses();
      setStatus('Wyczyszczono.');
    }

    function setStatus(msg, error = false) {
      const el = document.getElementById('status');
      el.textContent = msg;
      el.className = error ? 'status error' : 'status';
    }

    // Initial render
    renderClasses();
  </script>
</body>
</html>)rawliteral";
