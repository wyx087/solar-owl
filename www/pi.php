<html>
<head>
<meta http-equiv="refresh" content="15" > 
<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7; IE=EmulateIE9">
<meta content="text/html;charset=utf-8" http-equiv="Content-Type">
<meta content="utf-8" http-equiv="encoding">
<script type="text/javascript"
  src="dygraph-combined.js"></script>
<title>Solar generation and usage graph </title>
</head>
<body bgcolor="#000000"> 
    <meta name="viewport" content="width=320px">
    
    <div id="mygraph"
        style="width:470px; height:300px;"></div>
    
    <script type="text/javascript">
        // On page load, run: 
        window.onload = function(){
            setTimeout(function() { drawgraph(); }, 10);
        };
        
        // Drawing the graph 
        function drawgraph() {
            g = new Dygraph(
            document.getElementById("mygraph"),
            "log/solarshort.csv", 
                {
                    labels: [ "Time", "Usage", "Generating", "Exporting" ],
                    colors: ['#FFFFFF', '#00E000', '#FF0000'],
                    "axisLabelColor": "white",
                    series: {
                       Generating: { fillGraph: true }
                    }
                }
            );
        };
        
        // For zoom buttons 
        function selectLasthours(hours) {
        var range = g.xAxisRange();
        var maxX = range[1];
        var minX = maxX - hours * 60 * 60 * 1000;
        g.updateOptions({
            dateWindow: [minX, maxX],
            rollPeriod: hours
        });
        }
        
    </script>

</body>
</html>

