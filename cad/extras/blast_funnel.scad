$fn=360;

// funnel incase i need it to blast the pvc crossbars

top_dia=100;
pvc_dia=22;
inner_dia=15;
wall_dia=4; //2mm thick walls
funnel_height=50;
funnel_lenght=25;
inner_funnel_height=15;

difference()
{
body();
negs();
}

difference()
{
translate([0,0,funnel_height])
cylinder(h=inner_funnel_height,d1=pvc_dia+wall_dia,d2=inner_dia-1);

translate([0,0,funnel_height-1])
cylinder(h=inner_funnel_height+2,d1=pvc_dia+.5,d2=inner_dia-wall_dia);
}

module body()
{
cylinder(h=funnel_height, d1=top_dia, d2=pvc_dia+wall_dia);

translate([0,0,funnel_height])

cylinder(h=funnel_lenght,d=pvc_dia+wall_dia);
}

module negs()
{

cylinder(h=funnel_height, d1=top_dia-wall_dia, d2=pvc_dia);

translate([0,0,funnel_height-1])

cylinder(h=funnel_lenght+2,d=pvc_dia);
}
