const char MAIN_page[] PROGMEM = R"=====(
  <!doctype html>
  <html>
  <head>
    <title>ESP32 Feather BME680 WiFi Program</title>
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
      <div style="text-align:center;"><img src="https://www.sv-zanshin.com/r/images/site/gif/zanshinkanjitiny.gif" alt="Zanshin Logo"> <b>Zanshin</b></div>
      <div class="chart-container" position: relative; height:350px; width:100%">
          <canvas id="Chart" width="400" height="400"></canvas>
      </div>
  <div>
    <table id="dataTable">
      <tr><th>Time</th><th>Supply Voltage</th><th>Temperature &deg;C</th><th>Relative Humidity %</th><th>Pressure Pa</th></tr>
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
                  label: "SupplyVoltage",
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
                  label: "Pressure",
                  fill: false,
                  backgroundColor: 'rgba(156, 18, 243 , 1)',
                  borderColor: 'rgba(156, 18, 243 , 1)',
                  data: Pvalues,
              }],
          },
          options: {
              title: {
                      display: true,
                      text: "Real-Time BME680 Data Display"
                  },
              maintainAspectRatio: false,
              elements: {
              line: {
                      tension: 0.5  // Smooth data lines
                  }
              }
//			  ,
//              scales: {
//                      yAxes: [{
//                          ticks: {
//                              beginAtZero:true
//                          }
//                      }]
//              }
          }
      });
  }
  window.onload = function() {
    console.log(new Date().toLocaleTimeString());
  };
  
  // Get a new measurement every 5 Seconds 
  setInterval(function() {
    getData();
  }, 5000); // 5000ms update rate
   
  function getData() {
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function() {
      if (this.readyState == 4 && this.status == 200) {
       //Push the data in array
    var time = new Date().toLocaleTimeString();
    var txt = this.responseText;
    var obj = JSON.parse(txt); //Ref: https://www.w3schools.com/js/js_json_parse.asp
        SupplyVoltage.push(obj.SupplyVoltage);
        Tvalues.push(obj.Temperature);
        Hvalues.push(obj.Humidity);
        timeStamp.push(time);
        showGraph();  //Update Graphs
      var table = document.getElementById("dataTable");
      var row = table.insertRow(1); //Add after headings
      var cell1 = row.insertCell(0);
      var cell2 = row.insertCell(1);
      var cell3 = row.insertCell(2);
      var cell4 = row.insertCell(3);
      var cell5 = row.insertCell(4);
      cell1.innerHTML = time;
      cell2.innerHTML = obj.SupplyVoltage;
      cell3.innerHTML = obj.Temperature;
      cell4.innerHTML = obj.Humidity;
      cell5.innerHTML = obj.Pressure;
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