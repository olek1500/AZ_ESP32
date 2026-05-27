#ifndef _HTML_H_
#define _HTML_H_

static const char PROGMEM INDEX_HTML[] =R"rawliteral(
  <html>
  <head>
    <title>ESP32-CAM Robot</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body { font-family: Arial; text-align: center; margin:0px auto; padding-top: 10px;}
      table { margin-left: auto; margin-right: auto; padding-top: 130px; } /* padding-top obnizamy przyciski do sterowania */
      td { padding: 8 px; }
      .button {
        background-color:black;
        color: white;
        padding: 10px 20px;
        text-align: center;
        text-decoration: none;
        display: inline-block;
        font-size: 18px;
        margin: 6px 3px;
        cursor: pointer;
        -webkit-touch-callout: none;
        -webkit-user-select: none;
        -khtml-user-select: none;
        -moz-user-select: none;
        -ms-user-select: none;
        user-select: none;
        -webkit-tap-highlight-color: rgba(0,0,0,0);
        border-radius:15%;
        box-shadow: 3px 3px #888888;
      }
      .button:active {
      transform: translate(5px,5px);
      box-shadow: none; 
      }
      .img-wrapper {
        position: relative;
        display: inline-block;
        max-width: 80%;
        margin-top: 10px;
      }
      .img-wrapper img {
        width: 100%;
        height: auto;
        transform: rotate(-180deg);
        display: block;
      }
      #crosshair {
        position: absolute;
        width: 30px;
        height: 30px;
        border: 3px solid #00FF00; /* Kolor celownika - Zielony neonowy */
        border-radius: 50%;
        transform: translate(-50%, -50%);
        pointer-events: none;
        display: none;
        box-shadow: 0 0 5px rgba(0,0,0,0.8);
      }
      .noselect {
      -webkit-touch-callout: none; /* iOS Safari */
        -webkit-user-select: none; /* Safari */
         -khtml-user-select: none; /* Konqueror HTML */
           -moz-user-select: none; /* Firefox */
            -ms-user-select: none; /* Internet Explorer/Edge */
                user-select: none; /* Non-prefixed version, currently
                                      supported by Chrome and Opera */
    }
    .slidecontainer {
      width: 100%;
    }
    .slider {
      -webkit-appearance: none;
      width: 100%;
      height: 15px;
      border-radius: 5px;
      background: #d3d3d3;
      outline: none;
      opacity: 0.7;
      -webkit-transition: .2s;
      transition: opacity .2s;
    }
    .slider:hover {
      opacity: 1;
    }
    .slider::-webkit-slider-thumb {
      -webkit-appearance: none;
      appearance: none;
      width: 25px;
      height: 25px;
      border-radius: 50%;
      background: red;
      cursor: pointer;
    }
    .slider::-moz-range-thumb {
      width: 25px;
      height: 25px;
      border-radius: 50%;
      background: red;
      cursor: pointer;
    }
    </style>
  </head>
  <body>
    <h1>ESP32-CAM Robot</h1>
    <h3 id="status-panel" style="color:#4CAF50; margin: 5px 0;">Status: Wyłączony</h3>
    <div class="img-wrapper">
      <img src="" id="photo" >
      <div id="crosshair"></div>
    </div>
    <table>
      <tr><td colspan="3" align="center"><button class="button" onmousedown="toggleCheckbox('forward');" ontouchstart="toggleCheckbox('forward');" onmouseup="toggleCheckbox('stop');" ontouchend="toggleCheckbox('stop');">Forward</button></td></tr>
      <tr><td align="center"><button class="button" onmousedown="toggleCheckbox('left');" ontouchstart="toggleCheckbox('left');" onmouseup="toggleCheckbox('stop');" ontouchend="toggleCheckbox('stop');">Left</button></td><td align="center"><button class="button" onmousedown="toggleCheckbox('stop');" ontouchstart="toggleCheckbox('stop');">Stop</button></td><td align="center"><button class="button" onmousedown="toggleCheckbox('right');" ontouchstart="toggleCheckbox('right');" onmouseup="toggleCheckbox('stop');" ontouchend="toggleCheckbox('stop');">Right</button></td></tr>
      <tr><td colspan="3" align="center"><button class="button" onmousedown="toggleCheckbox('backward');" ontouchstart="toggleCheckbox('backward');" onmouseup="toggleCheckbox('stop');" ontouchend="toggleCheckbox('stop');">Backward</button></td></tr>                   
      <tr><td colspan="3" align="center"><button class="button" onclick="toggleCheckbox('detection');" style="background-color: #4CAF50;">Tryb Detekcji</button></td></tr>                   
      <tr><td colspan="3" align="center"><button class="button" onclick="toggleCheckbox('center');" style="background-color: #2196F3;">Wyśrodkuj</button></td></tr>                   

    </table>
    
   <script>
   function toggleCheckbox(x) {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/action?go=" + x, true);
     xhr.send();
   }
   function sendButtonInput(key, value) 
   {
     var xhr = new XMLHttpRequest();
     xhr.open("GET", "/action?" + key +"=" + value, true);
     xhr.send();
   }
   window.onload = document.getElementById("photo").src = window.location.href.slice(0, -1) + ":81/stream";

   // Pętla pobierająca w tle status od ESP32 co 500 ms
   setInterval(function() {
     var xhr = new XMLHttpRequest();
     xhr.onreadystatechange = function() {
       if (this.readyState == 4 && this.status == 200) {
         document.getElementById("status-panel").innerHTML = "Status: " + this.responseText;
       }
     };
     xhr.open("GET", "/status", true);
     xhr.send();
   }, 500);

   // Pętla odświeżająca na żywo pozycję celownika co 200 ms
   setInterval(function() {
     var xhr = new XMLHttpRequest();
     xhr.onreadystatechange = function() {
       if (this.readyState == 4 && this.status == 200) {
         var coords = this.responseText.split(",");
         if(coords.length == 2) {
           var x = parseInt(coords[0]);
           var y = parseInt(coords[1]);
           var ch = document.getElementById("crosshair");
           if (x >= 0 && y >= 0) {
             ch.style.display = "block";
             // Odbicie lustrzane 180° w obliczeniach, by zgrać się z rotacją kamerki
             ch.style.left = (100 - (x / 320 * 100)) + "%";
             ch.style.top = (100 - (y / 240 * 100)) + "%";
           } else {
             ch.style.display = "none";
           }
         }
       }
     };
     xhr.open("GET", "/target", true);
     xhr.send();
   }, 200);
  </script>
  </body>
</html>"
)rawliteral";


#endif /* HTML_H */
