<?php
$a = array("help");
if($argv[1]== "--help")
{
  echo "    Skript typu filtr (parse.phpv jazyce PHP 5.6)
    Načte ze standardního vstupu zdrojový kód v IPPcode18(viz sekce 6),
    zkontroluje lexikální a syntaktickou správnost kódu a vypíše na
    standardní výstup XML reprezentaci programu dle specifikace v sekci 3.1.
    
    Tento skript bude pracovat s těmito parametry:
      --help vypíše na standardní výstup nápovědu skriptu (nenačítá žádný vstup),
      kterou lze
      převzít ze zadání (lze odstranit diakritiku, případně přeložit do an
      gličtiny dle zvoleného jazyka
      dokumentace). Tento parametr nelze kombinovat s žádným dalším paramet
      rem, jinak skript
      ukončete s chybou 10.
    Chybové návratové kódy specifické pro analyzátor:
      21 - lexikální nebo syntaktická chyba zdrojového kódu zapsaného v IPPcode18.\n";
  
  exit(0);
}
else if($argc > 1|| ($argv[1]!= "--help" && $argc > 1))
  exit(10);
$fp = fopen('php://stdin','r');
$read = array($fp);
$write = NULL;
if(stream_select( $read, $write, $write, 0 ) === 1)
{
  $line = "";
  $explodedStr = "";
  $writer = new XMLWriter();
  $instrCounter = 0;
  $argcounter = 0;
  $writer->openURI('php://output'); 
  $writer->startDocument('1.0','UTF-8');
  $writer->setIndent(4);
  $writer->startElement('program');
  $writer->writeAttribute('language', 'IPPcode18');  

  $line = fgets($fp);
  //print(trim(strtolower(explode("#",$line)[0])));
  if(trim(strtolower(explode("#",$line)[0])) != ".ippcode18")
    exit(21);
  while (($line = fgets($fp)))
  {

    $line = str_replace("\n","",$line);
    $line = explode("#",$line);
    $array = explode(" ",$line[0]);
    $array = array_map('trim',$array);  //remove empty spaces
    $array = array_filter($array);  //remove empty element
    $array = array_values($array);  //reindex array
    if(empty(trim($line[0])) && !empty($line[1]))
      continue;
    if(!empty(trim($line[0])))
      Instructions($array[0],$array);
    if(!empty($line[1]))
      Comment($line[1]);
  }
  $writer->endDocument();
  $writer->flush();
  return 0;
}
function Comment($comment)
{
  //no stress
}

function CountOfTypeIsHigherThan($array,$intCount) {
  $count = 0;
  foreach($array as $key => $val) {
      if ($val !== '')
      {
        $count++;
      }
  }
  if($count!=$intCount)
  {
    fwrite(STDERR, "Syntax: a lot of params");
    exit(21);
  }
}

function WritableType($type)
{
  $type = strtolower($type);
  if($type != "gf"&&$type != "lf"&&$type != "tf")
  {
    fwrite(STDERR, "Syntax: var is has not correct type");
    exit(21);
  }
}

function startsWithNumber($str) 
{
  if(preg_match('/^\d/', $str) === 1)
  {
    fwrite(STDERR, "Syntax: var starts with numb");
    exit(21);
  }
}

function Instructions($instr, $typeAndVal)
{
  global $writer;
  global $instrCounter;
  global $argcounter;
  $instr = strtolower($instr);
  $instrCounter++;
  switch($instr)
  {
    case "move":
      $writer->startElement('instruction');
      $writer->writeAttribute('order', $instrCounter);
      $writer->writeAttribute('opcode', strtoupper($instr)); 
      CountOfTypeIsHigherThan($typeAndVal,3);    
      $pom = ParseTypeAndVal($typeAndVal[1]);
      startsWithNumber($pom[1]);
      WritableType($pom[0]);
      ParseTypeAndVal($typeAndVal[2]);
      $writer->endElement();
    break;
    case "createframe":
    case "pushframe":
    case "popframe":
    case "return":
    case "break":
      $writer->startElement('instruction');
      $writer->writeAttribute('order', $instrCounter);
      $writer->writeAttribute('opcode', strtoupper($instr)); 
      CountOfTypeIsHigherThan($typeAndVal,1);
      $writer->endElement();
    break;
    case "defvar":
    case "pops":
      $writer->startElement('instruction');
      $writer->writeAttribute('order', $instrCounter);
      $writer->writeAttribute('opcode', strtoupper($instr)); 
      CountOfTypeIsHigherThan($typeAndVal,2);
      $pom = ParseTypeAndVal($typeAndVal[1]);
      startsWithNumber($pom[1]);
      WritableType($pom[0]);
      $writer->endElement();
    break;
    case "call":
    case "label":
    case "jump":
      $writer->startElement('instruction');
      $writer->writeAttribute('order', $instrCounter);
      $writer->writeAttribute('opcode', strtoupper($instr)); 
      CountOfTypeIsHigherThan($typeAndVal,2);
      ParseLabel($typeAndVal[1]);
      $writer->endElement();
    break;
    case "add":
    case "sub":
    case "mul":
    case "idiv":
    case "lt":
    case "gt":
    case "eq":
    case "and":
    case "or":
      $writer->startElement('instruction');
      $writer->writeAttribute('order', $instrCounter);
      $writer->writeAttribute('opcode', strtoupper($instr)); 
      CountOfTypeIsHigherThan($typeAndVal,4);
      $pom = ParseTypeAndVal($typeAndVal[1]);
      startsWithNumber($pom[1]);
      WritableType($pom[0]);
      ParseTypeAndVal($typeAndVal[2]);
      ParseTypeAndVal($typeAndVal[3]);
      $writer->endElement();
    break;
    case "stri2int":
    case "concat":
    case "getchar":
    case "setchar":
    $writer->startElement('instruction');
    $writer->writeAttribute('order', $instrCounter);
    $writer->writeAttribute('opcode', strtoupper($instr)); 
    CountOfTypeIsHigherThan($typeAndVal,4);
    $pom = ParseTypeAndVal($typeAndVal[1]);
    startsWithNumber($pom[1]);
    WritableType($pom[0]);
    ParseTypeAndVal($typeAndVal[2]);
    ParseTypeAndVal($typeAndVal[3]);
    $writer->endElement();
    break;
    case "int2char":
    case "type":
    case "not":
      $writer->startElement('instruction');
      $writer->writeAttribute('order', $instrCounter);
      $writer->writeAttribute('opcode', strtoupper($instr)); 
      CountOfTypeIsHigherThan($typeAndVal,3);
      $pom = ParseTypeAndVal($typeAndVal[1]);
      startsWithNumber($pom[1]);
      WritableType($pom[0]);
      ParseTypeAndVal($typeAndVal[2]);
      $writer->endElement();
    break;
    case "read":
      $writer->startElement('instruction');
      $writer->writeAttribute('order', $instrCounter);
      $writer->writeAttribute('opcode', strtoupper($instr)); 
      CountOfTypeIsHigherThan($typeAndVal,3);
      $pom = ParseTypeAndVal($typeAndVal[1]);
      startsWithNumber($pom[1]);
      WritableType($pom[0]);
      $writer->startElement('arg'.++$argcounter);
      $writer->writeAttribute('type',"type");
      $writer->text($typeAndVal[2]);
      $writer->endElement();
      $writer->endElement();
    break;
    case "write":
    case "pushs":
    case "dprint":
      $writer->startElement('instruction');
      $writer->writeAttribute('order', $instrCounter);
      $writer->writeAttribute('opcode', strtoupper($instr)); 
      CountOfTypeIsHigherThan($typeAndVal,2);
      ParseTypeAndVal($typeAndVal[1]);
      $writer->endElement();
    break;
    case "strlen":
      $writer->startElement('instruction');
      $writer->writeAttribute('order', $instrCounter);
      $writer->writeAttribute('opcode', strtoupper($instr)); 
      CountOfTypeIsHigherThan($typeAndVal,3);
      $pom = ParseTypeAndVal($typeAndVal[1]);
      WritableType($pom[0]);
      ParseTypeAndVal($typeAndVal[2]);
      $writer->endElement();
    break;
    case "jumpifeq":
    case "jumpifneq":
      $writer->startElement('instruction');
      $writer->writeAttribute('order', $instrCounter);
      $writer->writeAttribute('opcode', strtoupper($instr)); 
      CountOfTypeIsHigherThan($typeAndVal,4);
      ParseLabel($typeAndVal[1]);
      ParseTypeAndVal($typeAndVal[2]);
      ParseTypeAndVal($typeAndVal[3]);
      $writer->endElement();
    break;
    default:
      fwrite(STDERR, "Lex error");
    exit(21);
    break;
  }
  $argcounter = 0;
}

function ParseLabel($str)
{
  global $writer;
  global $argcounter;
  $argcounter++;
  $writer->startElement('arg'.$argcounter);
  $writer->writeAttribute('type','label');
  ExpectedLabel($str);
  $writer->text($str);
  $writer->endElement();
}

function ExpectedLabel($str)
{
  if(strpos($str,'@')!== false)
  {
    fwrite(STDERR, "Syntax: label has got temporary");
    exit(21);
  }
  if(is_numeric(substr($str,0,1)))
  {
    fwrite(STDERR, "Syntax: label starts with number");
    exit(21);
  }
}

function ParseTypeAndVal($str)
{
  global $writer;
  global $argcounter;
  $argcounter++;
  $writer->startElement('arg'.$argcounter);
  $arr = explode("@",$str);
  if(strtolower($arr[0]) == "gf"||strtolower($arr[0]) == "lf"||strtolower($arr[0]) == "tf")
  {
    //nefungovala funkce ctype_lower tak jsem to naifoval...
    if(strtolower($arr[0]) == "gf")
      if($arr[0]!= "GF")
      {
        fwrite(STDERR, "Syntax: frame is not case upper");
        exit(21);
      }
    if(strtolower($arr[0]) == "lf")
      if($arr[0]!= "LF")
      {
        fwrite(STDERR, "Syntax: frame is not case upper");
        exit(21);
      }
    if(strtolower($arr[0]) == "tf")
      if($arr[0]!= "TF")
      {
        fwrite(STDERR, "Syntax: frame is not case upper");
        exit(21); 
      }
    TemporaryCheck($arr[1]);
    $writer->writeAttribute('type', 'var');
    $writer->text(strtoupper($arr[0])."@".$arr[1]);
  }
  else 
  {
    if($arr[0] == "int")
    {
      HaveIntGotMinusInside($arr[1]);
    }
    else if($arr[0] == "bool")
    {
      BoolValues($arr[1]);
    }
    else if($arr[0] == "string")
    {
      CheckEscapeSequences($arr[1]);
    }
    else exit(21);
    $writer->writeAttribute('type',$arr[0]);
    $text =$arr[1];
    if(substr_count($str,"@")>1)
    {
      for ($i = 2;$i<=substr_count($str,"@");$i++)
      {
        $text.="@";
        $text.=$arr[$i];
      }
    }
    if(strpos($text, '@') !== false && $arr[0] != "string")
    {
      fwrite(STDERR, "Syntax: int contains @");
      exit(21);
    }
    $writer->text($text);
  }
  $writer->endElement();
  return array($arr[0],$arr[1]);
}

function TemporaryCheck($str)
{
  if(substr($str, 0, 1)== '.')
  {
    fwrite(STDERR, "Syntax: temporary starts with . dot");
    exit(21);
  }
}

function BoolValues($str)
{
  if(strtolower($str) != "true" && strtolower($str)!= "false")
  {
    fwrite(STDERR, "Syntax: bool values are not good");
    exit(21);
  }
}

function HaveIntGotMinusInside($str)
{
  if($str== '')
  {
    fwrite(STDERR, "Syntax: int is empty");
    exit(21);
  }
  if(strpos($str, '.') !== false)
  {
    fwrite(STDERR, "Syntax: int contains . dot");
    exit(21);
  }
  if(strpos($str, '-') !== false||strpos($str, '+') !== false) 
  {
    if(substr($str, 0, 1)== '-' || substr($str,0,1)== '+')
    {
      if(!is_numeric(substr($str, 1, 1)))
      {
        fwrite(STDERR, "Syntax: second character is not number");
        exit(21);
      }
    }
    else
    {
      fwrite(STDERR, "Syntax: int contains - inside");
      exit(21);
    }
  }
}
function CheckEscapeSequences($str)
{
  $str = explode("\\",$str);
  for($i = 1;$i<count($str);$i++)
  {
    if(!is_numeric(substr($str[$i],0,3)))
    {
      fwrite(STDERR, "Syntax: escape sequences");
      exit(21);
    }
  }
}
?>
