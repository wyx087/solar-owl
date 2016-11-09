<html>
<head>

<meta http-equiv="Cache-Control" content="no-cache, no-store, must-revalidate" />
<meta http-equiv="Pragma" content="no-cache" />
<meta http-equiv="Expires" content="0" />

<meta content="text/html;charset=utf-8" http-equiv="Content-Type">
<meta content="utf-8" http-equiv="encoding">
<script type="text/javascript" src="dygraph-combined.js"></script>

<title>Solar generation and usage </title>
</head>
<body>
<meta name="viewport" content="width=610">

<a href="long.php">Longer text log</a> or <a href="g.php">Full and larger graph</a> 
<br>

<div id="mygraph"
    style="width:600; height:480;"></div>
<script type="text/javascript">
    
    g = new Dygraph(
    document.getElementById("mygraph"),
    "log/solarshort.csv", 
        {
            labels: [ "Time", "Usage", "Generating", "Exporting" ],
            colors: ['#000000', '#00E000', '#FF0000'],
            series: {
                Generating: { fillGraph: true }
            }
        }
    );
    
</script>

<?php
function read_last_lines($fp, $num){
    $idx   = 0;
    $lines = array();
    while(($line = fgets($fp)))
    {
        $lines[$idx] = $line;
        $idx = ($idx + 1) % $num;
    }
    $p1 = array_slice($lines,    $idx);
    $p2 = array_slice($lines, 0, $idx);
    $ordered_lines = array_merge($p1, $p2);
    return $ordered_lines;
}

$file = "/var/tmp/solar.log";
$f = fopen($file, "r");
$lines = read_last_lines($f, 300);
fclose($f);

$lines = array_reverse($lines);
foreach($lines as $child) {
    echo("<pre>");
    print($child);
    echo("</pregt;");
}

?>
</body>
</html>

