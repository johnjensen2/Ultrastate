//stems for control
$fn=360;


difference()
{
hull()
{
translate([6,6,0])
cylinder(h=55,d=3);

translate([-6,6,0])
cylinder(h=55,d=3);

translate([6,-6,0])
cylinder(h=55,d=3);

translate([-6,-6,0])
cylinder(h=55,d=3);
}

translate([-2.54,-1.27,-.1])
dupont_6_pack();

translate([0,0,14])
cylinder(h=50,d=12);
}


module dupont_6_pack()
{
for(a=[0:2.54:2.54])
{

for(b=[0:2.54:5.08])
{
translate([b,a,0])
dupont_single();
}
}
}



module dupont_single()
{
difference()
{
translate([-1.27,-1.27,0])
cube([2.54,2.54,15]);
//don't need the holes in this
//translate([0,0,-1])
//cylinder(h=16,d=2);
}
}