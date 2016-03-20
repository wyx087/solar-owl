<!DOCTYPE html>
<html>
<head>
<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7; IE=EmulateIE9">
<meta content="text/html;charset=utf-8" http-equiv="Content-Type">
<meta content="utf-8" http-equiv="encoding">
<script type="text/javascript"
  src="dygraph-combined.js"></script>
</head>
<body>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <br>
    
    <div id="mygraph"
        style="width:800px; height:600px;"></div>
    
    <p>
      <input type="button" value="Unzoom" onclick="unzoomGraph()">&nbsp;
    </p>
    <p><b>Select Data to show:</b><br/>
      <input type=checkbox id="0" checked onClick="change(this)">
      <label for="0"> Usage</label><br/>
      <input type=checkbox id="1" checked onClick="change(this)">
      <label for="1"> Generating</label><br/>
      <input type=checkbox id="2" checked onClick="change(this)">
      <label for="2"> Exporting</label>
    </p>

    
    <script type="text/javascript">
        g = new Dygraph(
        document.getElementById("mygraph"),
        "log/solar.csv", 
            {
            labels: [ "Time", "Usage", "Generating", "Exporting" ],
            rollPeriod: 4,
            zoomCallback : function(minDate, maxDate, yRange) {
                        showDimensions(minDate, maxDate, yRange);
              },
            }
        );
        
        // For Data selector 
        function setStatus() {
        document.getElementById("visibility").innerHTML =
          g.visibility().toString();
        }
        function change(el) {
        g.setVisibility(parseInt(el.id), el.checked);
        setStatus();
        }
        
        // For unzoom button 
        function unzoomGraph() {
        g.updateOptions({
          dateWindow: null,
          valueRange: null
        });
        }
    </script>

</body>
</html>

