$fn=360;
//possibly taper it so its a tap in
plug_id=15.7; //get from gauge
taper_amount = .5;
plug_height=10;
base_height=2.5;
screw_dia=5;
pipe_od=21.8;

difference()
{
union()
{
cylinder(h=base_height,d=pipe_od);

translate([0,0,base_height])
cylinder(h=plug_height-base_height,d1=plug_id,d2=plug_id-taper_amount);
}

hull()
{
translate([50,0,plug_height])
rotate([0,-90,0])
cylinder(h=100,d=screw_dia);

translate([50,0,plug_height/2+2])
rotate([0,-90,0])
cylinder(h=100,d=screw_dia);
}

//screw driver lip?

translate([0,15,2.5])
cube([14,10,2],center=true);

translate([0,-15,2.5])
cube([14,10,2],center=true);

translate([15,0,2.5])
cube([10,14,2],center=true);

translate([-15,0,2.5])
cube([10,14,2],center=true);
}
