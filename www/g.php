<!DOCTYPE html>
<html>
<head>
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
    
    <p><b>Select Data to show:</b></p>
    <p>
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
            rollPeriod: 16
            }
        );
        
        function setStatus() {
        document.getElementById("visibility").innerHTML =
          g.visibility().toString();
        }
        function change(el) {
        g.setVisibility(parseInt(el.id), el.checked);
        setStatus();
        }
    </script>

</body>
</html>

