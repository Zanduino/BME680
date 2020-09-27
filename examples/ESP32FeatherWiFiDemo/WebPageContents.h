/*!
  This character string "MAIN_page" is loaded into program memory and contains the web page that is
  served when a browser calls up the site. It uses the javascript from https://www.chartjs.org to
  generate a page with a dynamic chart and lists the data in tabular form underneath as well.

  Since this is used for demonstration purposes it has been kept basic and simple.

  Version | Date       | Developer  | Comments
  ------- | ---------- | ---------- | -------------------------------------------------------------
  1.0.2   | 2020-07-04 | SV-Zanshin | Issue #25 - implement clang-formatting
  1.0.1   | 2020-06-11 | SV-Zanshin | Changed URL to Zanshin kanji gif file
  1.0.0   | 2020-06-02 | SV-Zanshin | Ready to commit and publish as Issue #20

*/
const char MAIN_page[] PROGMEM = R"=====(
  <!doctype html>
  <html>
  <head>
    <title>ESP32FeatherWiFiDemo - BME680 demonstration program</title>
    <script src = "https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.7.3/Chart.min.js"></script>  
    <style>
    canvas{
      -moz-user-select: none;
      -webkit-user-select: none;
      -ms-user-select: none;
    }
    #dataTable {
      font-family: "Trebuchet MS", Arial, Helvetica, sans-serif;
      border-collapse: collapse;
      width: 100%;
    }
    #dataTable td, #dataTable th {
      border: 1px solid #ddd;
      padding: 8px;
    }
    #dataTable tr:nth-child(even){background-color: #f2f2f2;}
    #dataTable tr:hover {background-color: #ddd;}
    #dataTable th {
      padding-top: 12px;
      padding-bottom: 12px;
      text-align: left;
      background-color: #4CAF50;
      color: white;
    }
    </style>
  </head>
  <body>
      <div style="text-align:center;"><img src="https://zanduino.github.io/Images/zanshinkanjitiny.gif" alt="Zanshin Logo"> <b>Zanshin</b></div>
      <div class="chart-container" position: relative; height:350px; width:100%">
          <canvas id="Chart" width="400" height="400"></canvas>
      </div>
  <div>
    <table id="dataTable">
      <tr><th>Time</th><th>Supply (V)</th><th>Temperature (&deg;C)</th><th>Relative Humidity (%)</th><th>Altitude (m)</th></tr>
    </table>
  </div>
  <br>
  <br>  
  
  <script>
  //Graphs visit: https://www.chartjs.org
  var SupplyVoltage = [];
  var Tvalues = [];
  var Hvalues = [];
  var Pvalues = [];
  var timeStamp = [];
  function showGraph()
  {
      var ctx = document.getElementById("Chart").getContext('2d');
      var Chart2 = new Chart(ctx, {
          type: 'line',
          data: {
              labels: timeStamp,
              datasets: [{
                  label: "Supply",
                  fill: false,
                  backgroundColor: 'rgba( 243,18, 156 , 1)',
                  borderColor: 'rgba( 243, 18, 156 , 1)',
                  data: SupplyVoltage,
              },{
                  label: "Temperature",
                  fill: false,
                  backgroundColor: 'rgba( 243, 156, 18 , 1)',
                  borderColor: 'rgba( 243, 156, 18 , 1)',
                  data: Tvalues,
              },
              {
                  label: "Humidity",
                  fill: false,
                  backgroundColor: 'rgba(156, 18, 243 , 1)',
                  borderColor: 'rgba(156, 18, 243 , 1)',
                  data: Hvalues,
              }, 
			  {
                  label: "Altitude",
                  fill: false,
                  backgroundColor: 'rgba(60, 60, 60, 1)',
                  borderColor: 'rgba(60, 60, 60, 1)',
                  data: Pvalues,
              }],
          },
          options: {
              title: {
                      display: true,
                      text: "Real-Time BME680 Data Display with 10 second auto-refresh"
                  },
              maintainAspectRatio: false,
              elements: {
              line: {
                      tension: 0.5  // Smooth data lines
                  }
              }
			  ,
              scales: {
                      yAxes: [{
                          ticks: {
                              beginAtZero:true
                          }
                      }]
              }
          }
      });
  }
  window.onload = function() {
    console.log(new Date().toLocaleTimeString());
  };
  
  setInterval(function() { getData(); }, 10000); // Get a new measurement every 10 Seconds 
  function getData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
       //Push the data in array
    var time = new Date().toLocaleTimeString();
    var txt = this.responseText;
    var obj = JSON.parse(txt);
        SupplyVoltage.push(obj.SupplyVoltage);
        Tvalues.push(obj.Temperature);
        Hvalues.push(obj.Humidity);
		Pvalues.push(obj.Altitude);
        timeStamp.push(time);
        showGraph();
      var table = document.getElementById("dataTable");
      var row = table.insertRow(1);
      var cell1 = row.insertCell(0);
      var cell2 = row.insertCell(1);
      var cell3 = row.insertCell(2);
      var cell4 = row.insertCell(3);
      var cell5 = row.insertCell(4);
      cell1.innerHTML = time;
      cell2.innerHTML = obj.SupplyVoltage;
      cell3.innerHTML = obj.Temperature;
      cell4.innerHTML = obj.Humidity;
      cell5.innerHTML = obj.Altitude;
      }
    };
    xhttp.open("GET", "readADC", true);
    xhttp.send();
  }
  </script>
  </body>
  </html><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
  <html xmlns="http://www.w3.org/1999/xhtml">
  <head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
  <title>Untitled Document</title>
  </head><body></body></html>
)=====";
