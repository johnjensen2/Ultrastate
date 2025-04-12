//stems for control
$fn=360;

lmu_dia=15;
lmu_height=24;
lmu_shaft=9; //slightly larger that its real 8mm
lmu_shaft_height=50;

difference()
{
//base for now
translate([-(lmu_height+5)/2,-85,0])
cube([lmu_height+5,110,15]);

//bearings
translate([-12,15,0])
rotate([0,90,0])
cylinder(h=lmu_height,d=lmu_dia);

translate([-12,-75,0])
rotate([0,90,0])
cylinder(h=lmu_height,d=lmu_dia);

//shaft holes
translate([-20,15,0])
rotate([0,90,0])
cylinder(h=lmu_shaft_height,d=lmu_shaft);

translate([-20,-75,0])
rotate([0,90,0])
cylinder(h=lmu_shaft_height,d=lmu_shaft);


//stem holes do not change
translate([0,0,5])
stem();

translate([0,-60,5])
stem();

//wiring 
translate([-2.54,-1.27,-1])
dupont_6_pack();

translate([-2.54,-61.27,-1])
dupont_6_pack();

////zip ties holes
//zip_ties();
//translate([0,-30,0])
//zip_ties();

//linear_pot hole
translate([-2,-30.65,-1])
cube([4,1.3,20]);

//linear pot groove 
hull()
{
translate([25,-30.65,-1])
cylinder(h=8,d1=15,d2=10);

translate([-25,-30.65,-1])
cylinder(h=8,d1=15,d2=10);
}

}



module zip_ties()
{
translate([8.5,-6.5,-1])
cube([5,3,20]);

translate([-13.5,-6.5,-1])
cube([5,3,20]);

translate([8.5,-11.5-lmu_dia,-1])
cube([5,3,20]);

translate([-13.5,-11.5-lmu_dia,-1])
cube([5,3,20]);
}

module stem()
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