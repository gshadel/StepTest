const char ctrlpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      html {
        font-family: Arial;
        display: inline-block;
        margin: 0px auto;
        text-align: center;
      }
      
      .slidecontainer {
        width: 100%;
      }
      .slider {
        -webkit-appearance: none;
        width: 50%;
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
        background: #4CAF50;
        cursor: pointer;
      }
      .slider::-moz-range-thumb {
        width: 25px;
        height: 25px;
        border-radius: 50%;
        background: #4CAF50;
        cursor: pointer;
      }     
      .toggle {
        position : relative ;
        display : inline-block;
        width : 100px;
        height : 52px;
        background-color: red;
        border-radius: 30px;
        border: 2px solid gray;
      }
               
      /* After slide changes */
      .toggle:after {
        content: '';
        position: absolute;
        width: 50px;
        height: 50px;
        border-radius: 50%;
        background-color: gray;
        top: 1px;
        left: 1px;
        transition:  all 0.5s;
      } 
      p {
        font-family: Arial, Helvetica, sans-serif;
        font-weight: bold;
      }
      .checkbox:checked + .toggle::after {
        left : 49px;
      }
      .checkbox:checked + .toggle {
        background-color: green;
      }
      .checkbox {
        display : none
      }
  </style>
  </head>
  <body onload="sendOnOff(false); sendDir(false);sendData(6.0);">
    <h1>Planetarium Eclipic Motor</h1>
    <center>Ver 1.0</center>
    <br><br>
    <div class="slidecontainer">
      <input type="range" min="3.0" max="9.0" value="6.0" step= "0.2" class="slider" id="stepRate">
      <p>Rate : <span id="rateSet"></span> RPM</p>
    <center>
      <br><br>

      <input type="checkbox" id="dir" class="checkbox" onclick="sendDir(this.checked);" />
      <label for="dir" class="toggle">
        <p>CCW &nbsp;&nbsp; CW</p>
      </label>

      &nbsp;&nbsp;&nbsp;
      
      <input type="checkbox" id="switch" class="checkbox" onclick="sendOnOff(this.checked);" />
      <label for="switch" class="toggle">
        <p>ON &nbsp;&nbsp; OFF</p>
      </label>
    </center>
    </div>
    <script>
      function sendData(pos) {
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            console.log(this.responseText);
          }
        };
        xhttp.open("GET", "setRate?Rate="+pos, true);
        xhttp.send();
      } 
      function sendOnOff(switchstat) {
        var sendThis = "F";
        if (switchstat) {
          sendThis = "O";
        }
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            console.log(this.responseText);
          }
        };
        xhttp.open("GET", "setOnOff?OnOff="+sendThis, true);
        xhttp.send();
      } 
      function sendDir(dirstat) {
        var sendThis = "R";
        if (dirstat) {
          sendThis = "L";
        }
        var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function() {
          if (this.readyState == 4 && this.status == 200) {
            console.log(this.responseText);
          }
        };
        xhttp.open("GET", "setDir?Dir="+sendThis, true);
        xhttp.send();
      } 
      var slider = document.getElementById("stepRate");
      var output = document.getElementById("rateSet");
      output.innerHTML = slider.value;
      slider.oninput = function() {
        output.innerHTML = this.value;
        sendData(output.innerHTML);
      }
    </script>
  </body>
</html>
)=====";