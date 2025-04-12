$fn=360;

base_height=5;
top_height=3;

section_height=2;

section(45);

module section(a)
{

    hull()
    {
    cylinder(h=section_height,d=base_height);

    translate([0,a,0])
    cylinder(h=section_height,d=base_height);
    }
    
    hull()
    {
    translate([0,0,section_height])
    cylinder(h=section_height,d1=base_height,d2=top_height);

    translate([0,a,section_height])
    cylinder(h=section_height,d1=base_height,d2=top_height);
    }

    hull()
    {
    translate([0,0,section_height*2])
    cylinder(h=section_height+5,d=top_height);
    
    translate([0,a,section_height*2])
    cylinder(h=section_height+5,d=top_height);
    }


    
    
 }