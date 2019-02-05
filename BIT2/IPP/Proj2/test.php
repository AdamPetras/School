<?php
$shortopts  = "h";
$shortopts .= "d:";  // Required value
$shortopts .= "r";
$shortopts .= "p:"; // Optional value
$shortopts .= "i:"; // These options do not accept values
$shortopts .= "c";

$longopts  = array(
    "help",
    "directory:",
    "recursive",
    "parse-script:",
    "int-script:",
    "clean",
);

$options = getopt($shortopts, $longopts);
$directory = "./";
$parsedir = "./parse.php";
$interpretdir = "./interpret.py";
$recursive = false;
$max_depth = 1;
if(array_key_exists("help",$options) || array_key_exists("h",$options))
{
     print
'     Skript (test.php v jazyce PHP 5.6) bude sloužit pro automatické testování
     postupné aplikace parse.php a interpret.py.
     Skript projde zadaný adresář s testy a využije je pro automatické
     otestování správné funkčnosti obou předchozích programů včetně vygenerování
     přehledného souhrnu v HTML 5 do standardního výstupu. Testovací skript nemusí u předchozích
     dvou skriptů testovat jejich dodatečnou funkčnost aktivovanou parametry příkazové řádky
     (s výjimkou potřeby parametru --source).

     Tento skript bude pracovat s těmito parametry:

•    --help viz společný parametr všech skriptů v sekci 2.2
•    --directory=path testy bude hledat v zadaném adresáři (chybí-li tento parametr,
     tak skript prochází aktuální adresář)
•    --recursive testy bude hledat nejen v zadaném adresáři, ale i rekurzivně 
     ve všech jehopodadresářích
•    --parse-script=file soubor se skriptem v PHP 5.6 pro analýzu zdrojového kódu v IPP-code18
     (chybí-li tento parametr, tak implicitní hodnotou jeparse.phpuložený v aktuálnímadresáři)
•    --int-script=file soubor se skriptem v Python 3.6 pro interpret XML reprezentace kódu
     v IPPcode18 (chybí-li tento parametr, tak implicitní hodnotou je interpret.py
     uložený v aktuálním adresáři)
•    --clean tento přepinač jsem si sam přidal pro čištění dat z testů'."\n";
     exit(0);
}
if(array_key_exists("directory",$options))
{
     $directory = $options["directory"];
}
else if(array_key_exists("d",$options))
{
     $directory = $options["d"];
}
if(array_key_exists("parse-script",$options))
{
     $parsedir = $options["parse-script"];
     
}
else if (array_key_exists("p",$options))
{
     $parsedir = $options["p"];
}
if(array_key_exists("int-script",$options))
{
     $interpretdir = $options["int-script"];
     
}
else if (array_key_exists("i",$options))
{
     $interpretdir = $options["i"];
}
if(array_key_exists("clean",$options))
{
     shell_exec("rm -rf out");
     shell_exec("rm -rf pyOut");
     shell_exec("rm -rf finalOut.html");
     exit(0);
}
else if (array_key_exists("c",$options))
{
     shell_exec("rm -rf out");
     shell_exec("rm -rf pyOut");
     shell_exec("rm -rf finalOut.html");
     exit(0);
}
if(array_key_exists("recursive",$options))
{
     $recursive = true;
}   
else if (array_key_exists("r",$options))
{
    $recursive = true;
}
shell_exec("rm -rf out");
shell_exec("mkdir out");

shell_exec("rm -rf pyOut");
shell_exec("mkdir pyOut");

shell_exec("rm -rf finalOut.html");

function cmp_by_optionNumber($a, $b, $c) 
{
    return $a[$c] - $b;
}

$phpdiffcomplete = 0;
$phpcodecomplete = 0;
$pythondiffcomplete = 0;
$pythondcodecomplete = 0;
$i = 1;
$myfile = fopen("finalOut.html", "w");
$txt = '<h2 style="color: #2e6c80;">IPP 2018 testing:</h2><table class="table" style = "border: 2px solid black; border-collapse: collapse; border: 4px solid black;">
<thead><tr style="border: 2px solid black"><th style="border: 2px solid black">Test</th>
<th style="border: 2px solid black">PHP error code</th><th style="border: 2px solid black">Python test(diff)</th><th style="border: 2px solid black">Python error code</th>
<th style="border: 2px solid black">Description</th></tr></thead><tbody><tr>';
fwrite($myfile,$txt);
//find . -maxdepth 3 -name "*.src" | sort -n | awk '{ print length $0 }' | sort -n|cut -d "." -f2-
if($recursive)
    $max_depth = 255;
$temporar = shell_exec("find ".$directory." -maxdepth ".$max_depth." -name \"*.src\"| sort -n | awk '{ print length, $0 }' | sort -n | cut -d \" \" -f2-");
$temporar = explode("\n",$temporar);
for (;$i <= shell_exec("find ".$directory." -maxdepth ".$max_depth." -name \"*.src\" | sort -n| awk '{ print length, $0 }' | sort -n | cut -d \" \" -f2- | wc -l");$i++)
{
    $dir = substr($temporar[$i-1],0,strlen($temporar[$i-1])-4);
    $resetName = str_replace("/","-",substr($temporar[$i-1],0,strlen($temporar[$i-1])-4));
    if(substr($resetName,0,2)== ".-")
    {
        $resetName = substr($resetName,2,strlen($resetName));
    }

    if(!file_exists($parsedir))
    {
        fwrite(STDERR, "Parser doesn't exist in this location " .$parsedir."\n");
        exit(-1);
    }
     exec("cat ".$dir.".src | php ".$parsedir." >> out/".$resetName.".inPy", $out, $ret_var);
     if($PHPErrCode == 0)
     {
        if(!file_exists($interpretdir))
        {
            fwrite(STDERR, "Interpret doesn't exist in this location " .$interpretdir."\n");
            exit(-1);
        }
        if(!file_exists($dir.".in"))
            shell_exec("touch ".$dir.".in");
        exec("python3 ".$interpretdir.' --source=out/'.$resetName.".inPy < ".$dir.".in >> pyOut/".$resetName.".myOut", $out, $ret_var);
        $PyErrCode = $ret_var;
        if(!file_exists($dir.".rc"))
            shell_exec('echo "0" >> '.$dir.".rc");
        $diffpythonReturnCode = shell_exec("cat ".$dir.".rc");
        if(!file_exists($dir.".out"))
            shell_exec("touch ".$dir.".out");
            $diffpython = shell_exec("diff pyOut/".$resetName.".myOut ".$dir.".out");
     }
    $diffphpcodecolor = "green";
     $diffpythoncodecolor = "green";
     if ($diffphpReturnCode != $PHPErrCode)
     {
          $diffphpcodecolor = "red";
          $PHPErrCode .= " expected ".$diffphpReturnCode;
     }
     else 
     {
          $phpcodecomplete++;
     }
     if ($diffpythonReturnCode != $PyErrCode)
     {
          $diffpythoncodecolor = "red";
          $PyErrCode .= " expected ".$diffpythonReturnCode;
     }
     else 
     {
          $pythoncodecomplete++;
     }

     $diffphpcolor = "green";
     if ($diffphp == "" || $PHPErrCode != 0)
     {
          $diffphp = "OK";
          $phpdiffcomplete++;
     }
     else
     {
          $diffphp = "ERROR";
          $diffphpcolor = "red";
     }

     $diffpythoncolor = "green";
     if ($diffpython == ""|| $PyErrCode != 0)
     {
          $diffpython = "OK";
          $pythondiffcomplete++;
     }
     else
     {
          $diffpython = "ERROR";
          $diffpythoncolor = "red";
     }
     //Test counter
     //PHP diff
     //PHP err code
     //Python diff
     //Python err code
     $txt = '<td align = "center" style="color: black;border: 2px solid black;">'.$i.'</td>
     <td align = "center" style="background-color: '.$diffphpcodecolor.';border: 2px solid black;">'.$PHPErrCode.'</td>
     <td align = "center" style="background-color: '.$diffpythoncolor.';border: 2px solid black;">'.$diffpython.'</td>
     <td align = "center" style="background-color: '.$diffpythoncodecolor.';border: 2px solid black;">'.$PyErrCode.'</td>
     </tr>';
     fwrite($myfile,$txt);
}
$i--;
$mul = 100;
if($i!=0)
{
    $txt = '
    <tr>
    <td align = "center" style="color: black;border: 2px solid black;">Complete</td>
    <td align = "center" style="color: black;border: 2px solid black;">'.$phpcodecomplete.'</td>
    <td align = "center" style="color: black;border: 2px solid black;">'.$pythondiffcomplete.'</td>
    <td align = "center" style="color: black;border: 2px solid black;">'.$pythoncodecomplete.'</td>
    </tr>
    <tr>
    <td align = "center" style="color: black;border: 2px solid black;">Percentage</td>
    <td align = "center" style="color: black;border: 2px solid black;">'.round($phpcodecomplete/$i*$mul,1).'%</td>
    <td align = "center" style="color: black;border: 2px solid black;">'.round($pythondiffcomplete/$i*$mul,1).'%</td>
    <td align = "center" style="color: black;border: 2px solid black;">'.round($pythoncodecomplete/$i*$mul,1).'%</td>
    </tr>
    </tbody>
    </table>';
fwrite($myfile,$txt);
}
fclose($myfile);
?>
