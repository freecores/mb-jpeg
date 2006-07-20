setMode -bs
setCable -port auto
identify
identifyMPM
assignFile -p 3 -file "implementation/download.bit"
program -p 3 
quit
