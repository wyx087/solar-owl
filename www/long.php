<html>
<head>

<meta http-equiv="Cache-Control" content="no-cache, no-store, must-revalidate" />
<meta http-equiv="Pragma" content="no-cache" />
<meta http-equiv="Expires" content="0" />

<meta content="text/html;charset=utf-8" http-equiv="Content-Type">
<meta content="utf-8" http-equiv="encoding">


<title>Solar generation and usage </title>
</head>
<body>
<meta name="viewport" content="width=500">

<a href="index.php">Back</a>

<?php
header('Expires: Sun, 01 Jan 2014 00:00:00 GMT');
header('Cache-Control: no-store, no-cache, must-revalidate');
header('Cache-Control: post-check=0, pre-check=0', FALSE);
header('Pragma: no-cache');

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
$lines = read_last_lines($f, 20000);
fclose($f);

$lines = array_reverse($lines);
foreach($lines as $child) {
    echo("<pre>");
    print($child);
    echo("</pregt;");
}

$file = "/var/tmp/solar-1.log";
$f = fopen($file, "r");
$lines = read_last_lines($f, 20000);
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

