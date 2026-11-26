# Decode only VHDMP records for the named leaf. The ETL stays local.
param([Parameter(Mandatory)][string]$Etl, [Parameter(Mandatory)][string]$Disk,
      [Parameter(Mandatory)][datetimeoffset]$Until, [Parameter(Mandatory)][string]$Output)
$ErrorActionPreference='Stop'
$q=[System.Diagnostics.Eventing.Reader.EventLogQuery]::new(
    (Resolve-Path -LiteralPath $Etl).Path,[System.Diagnostics.Eventing.Reader.PathType]::FilePath,
    "*[System[Provider[@Name='Microsoft-Windows-VHDMP'] and (EventID=1001 or EventID=1002 or EventID=1010 or EventID=1011)]]")
$reader=[System.Diagnostics.Eventing.Reader.EventLogReader]::new($q)
$reader.BatchSize=512
$out=[IO.StreamWriter]::new($Output,$false,[Text.UTF8Encoding]::new($false))
try{
    $out.WriteLine("utc_ticks`tid`tactivity`tio`tsrb`toffset`tlength`tstatus")
    while($e=$reader.ReadEvent()){
        try{
            $t=$e.TimeCreated.ToUniversalTime()
            if($t -gt $Until.UtcDateTime){continue}
            $p=$e.Properties
            if($p[0].Value -ne $Disk){continue}
            $i=1;$s=''
            if($e.Id -ge 1010){$i=2;$s=$p[6].Value}
            $out.WriteLine(('{0},{1},{2},{3},{4},{5},{6},{7}' -f $t.Ticks,$e.Id,$e.ActivityId,
                $p[$i].Value,$p[$i+1].Value,$p[$i+2].Value,$p[$i+3].Value,$s).Replace(',',"`t"))
        }finally{$e.Dispose()}
    }
}finally{$out.Dispose();$reader.Dispose()}
