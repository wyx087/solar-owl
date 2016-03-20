<!DOCTYPE html>
<html>
<head>
<script type="text/javascript"
  src="dygraph-combined.js"></script>
</head>
<body>
<meta name="viewport" content="width=device-width, initial-scale=1">

<br><br>

<div id="mygraph"
  style="width:800px; height:600px;"></div>
<script type="text/javascript">
  g2 = new Dygraph(
    document.getElementById("mygraph"),
    "log/solar.csv", 
    {
	labels: [ "Time", "Usage", "Generating", "Exporting" ],
    rollPeriod: 16
    }
  );
</script>

</body>
</html>

