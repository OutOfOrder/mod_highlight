
while(<>){
 
  s/CDATA #IMPLIED/\%string; #IMPLIED/g;

  s/(<!ENTITY \% boolean)/<!ENTITY \% string \"CDATA\">\n$1/g;
  
  print;

}