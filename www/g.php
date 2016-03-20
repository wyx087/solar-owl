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
    <meta name="viewport" content="width=800px, initial-scale=1">
    <br>
    
    <?php
        function array_last_lines($list, $num){
            $idx   = 0;
            $lines = $list;
            $p1 = array_slice($lines,    $idx);
            $p2 = array_slice($lines, 0, $idx);
            $ordered_lines = array_merge($p1, $p2);
            return $ordered_lines;
        }
        echo "<select name='files' onchange=\"newcsvfile()\">";
        $files = array_map("htmlspecialchars", scandir("/var/www/log"));
        $last_files = array_last_lines($files, 2);
        foreach ($last_files as $file)
        echo "<option value='$file'>$file</option>";
        echo "</select>";
    ?>
    
    <div id="mygraph"
        style="width:800px; height:600px;"></div>
    
    <p>
      <input type="button" value="Unzoom" onclick="unzoomGraph()">&nbsp;
      <input type="button" value="Last 8 hours" onclick="selectLasthours(8)">&nbsp;
      <input type="button" value="Last 4 hours" onclick="selectLasthours(4)">&nbsp;
      <input type="button" value="Last 2 hours" onclick="selectLasthours(2)">&nbsp;
      <input type="button" value="Last hour" onclick="selectLasthours(1)">&nbsp;
    </p>
    <p><b>Select Data to show:</b><br/>
      <input type=checkbox id="0" checked onClick="change(this)">
      <label for="0"> <font color="#000000">Usage</font></label><br/>
      <input type=checkbox id="1" checked onClick="change(this)">
      <label for="1"> <font color="#00E000">Generating</font></label><br/>
      <input type=checkbox id="2" checked onClick="change(this)">
      <label for="2"> <font color="#FF0000">Exporting</font></label>
    </p>

    
    <script type="text/javascript">
        // On page load, run: 
        window.onload = function(){
           setTimeout(function() { selectLasthours(2); }, 500);
        };
        
        // Drawing the graph 
        g = new Dygraph(
        document.getElementById("mygraph"),
        "log/solar.csv", 
            {
                labels: [ "Time", "Usage", "Generating", "Exporting" ],
                colors: ['#000000', '#00E000', '#FF0000'],
                rollPeriod: 2
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
        
        // For zoom buttons 
        function unzoomGraph() {
        g.updateOptions({
          dateWindow: null,
          valueRange: null
        });
        }
        function selectLasthours(hours) {
        var range = g.xAxisRange();
        var maxX = range[1];
        var minX = maxX - hours * 60 * 60 * 1000;
        g.updateOptions({
            dateWindow: [minX, maxX],
        });
        }
        
        
        
    </script>

</body>
</html>

