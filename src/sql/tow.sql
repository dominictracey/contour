--
-- tow.sql - 
--   This program is free software. You can use, copy and modify it under
-- the terms of the GPL. See the file COPYING for the full GPL license.
--
-- (c) 2000 Aquilon Technology Systems, Inc.
--
-- Created: Sun Nov 11 22:25:46 EST 2001
--

-- I am going to factor out fisheries specific information. This is for dragging.


drop table damage;
create table damage
(
	damageid integer primary key,
	short_name varchar,
	description varchar
);

drop sequence seq_damage;
create sequence seq_damage start 1;
insert into damage values (nextval('seq_damage'),'belly out', 'belly peeled back or lost');
insert into damage values (nextval('seq_damage'),'rock', 'dropped a rock through cod-end');

 
drop table effort_tow;
create table effort_tow
(
	effortid integer references effort(effortid),
	netid integer references nets(netid),
	damageid integer references damage(damageid),
	wire_fathoms integer,
	groundcables_fathoms integer,
	spread_feet integer
);

drop function tow_add(int4,int4,int4,int4,int4,int4);
create function tow_add(int4,int4,int4,int4,int4,int4)
	returns int4 as '
	declare
		id alias for $1;
		net alias for $2;
		damage alias for $3;		
		wire alias for $4;		
		gc alias for $5;		
		sf alias for $6;		
	begin
		insert into effort_tow (effortid, netid, damageid, wire_fathoms, groundcables_fathoms, spread_feet)
			values (id, net, damage, wire, gc, sf); 
		return id;
	end;
'language 'plpgsql';

drop function tow_edit(int4,int4,int4,int4,int4,int4);
create function tow_edit(int4,int4,int4,int4,int4,int4)
	returns int4 as '
	declare
		id alias for $1;
		net alias for $2;
		damage alias for $3;		
		wire alias for $4;		
		gc alias for $5;		
		sf alias for $6;		
	begin
		update effort_tow set netid=net, damageid=damage, wire_fathoms=wire, groundcables_fathoms=gc, spread_feet=sf where effortid=id;
		return id;
	end;
'language 'plpgsql';


