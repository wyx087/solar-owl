<html>
<head>
<meta http-equiv="refresh" content="15" > 
<meta http-equiv="Cache-Control" content="no-cache, no-store, must-revalidate" />
<meta http-equiv="Pragma" content="no-cache" />
<meta http-equiv="Expires" content="0" />

<meta content="text/html;charset=utf-8" http-equiv="Content-Type">
<meta content="utf-8" http-equiv="encoding">
<script type="text/javascript" src="dygraph-combined.js"></script>

<title>Solar generation and usage graph </title>
</head>
<body bgcolor="#000000"> 
    <meta name="viewport" content="width=320px">
    
    <div align="center"><font color=#c7cbe3><font size="4">
        <?php
        echo date("l . d-m-Y . h:i a");
        ?>
    </font></font></div>
    
    <div id="mygraph"
        style="width:470px; height:280px;"></div>
    
    <script type="text/javascript">
        // On page load, run: 
        window.onload = function(){
            setTimeout(function() { drawgraph(); }, 10);
        };
        
        // Drawing the graph 
        function drawgraph() {
            g = new Dygraph(
            document.getElementById("mygraph"),
            "log/solar_short.csv", 
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
        
    </script>
    

</body>
</html>

