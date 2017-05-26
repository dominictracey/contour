--
-- effort.sql - This is sort of a base class for things like tows, sets,
--  steaming about, laying to (each of which may have their own tables).
--
--   This program is free software. You can use, copy and modify it under
-- the terms of the GPL. See the file COPYING for the full GPL license.
--
-- (c) 2000 Aquilon Technology Systems, Inc.
--
-- Created: Mon Jul 30 23:41:44 EDT 2001
--

drop sequence seq_effort;
CREATE SEQUENCE seq_effort START 1;

drop table effort;
CREATE TABLE effort
(
	effortid int4 not null primary key default nextval('seq_effort'),
	date_start date,
	time_start time,
	date_end date,
	time_end time,
	lat_begin float4,
	long_begin float4,
	lat_end float4,
	long_end float4,
	max_lat float4,
	max_long float4,
	min_lat float4,
	min_long float4,
	guid varchar
);

drop table effort_trip;
create table effort_trip
(
	effortid int4 not null references effort(effortid),
	tripid int4 not null references trip(tripid)
);

--
--- effort_start - @REX need to calculate or accept guid
--	@REX assuming we are always going to add our efforts to our most current trip!
--
drop function effort_start();
create function effort_start()
	returns int4 as '
	declare
		currdate date;
		currtime time;
		currtrip int4;
	begin
		currdate := ''now'';
		currtime := ''now'';
		insert into effort (date_start, time_start) values
						 (currdate, currtime);
		select last_value into currtrip from seq_trip;
		insert into effort_trip (effortid,tripid) values (currval(''seq_effort''),currtrip);

		return currval(''seq_effort'');
	end;
'language 'plpgsql';

drop function effort_end(int4);
create function effort_end(int4)
	returns int4 as '
	declare
		eid alias for $1;
		currdate date;
		currtime time;
	begin
		currdate := ''now'';
		currtime := ''now'';
		update effort set date_end=currdate,
						  time_end=currtime
					  where effortid=eid;
		return eid;
	end;
'language 'plpgsql';

drop function effort_set_boundaries(int4,float8,float8,float8,float8,float8,float8,float8,float8);
create function effort_set_boundaries(int4,float8,float8,float8,float8,float8,float8,float8,float8)
	returns int4 as '
	declare
		eid alias for $1;
		startlat alias for $2;
		startlon alias for $3;
		endlat alias for $4;
		endlon alias for $5;
		maxlat alias for $6;
		maxlon alias for $7;
		minlat alias for $8;
		minlon alias for $9;
	begin
		update effort set 		
					lat_begin=startlat,
					long_begin=startlon,
					lat_end=endlat,
					long_end=endlon,
					max_lat=maxlat,
					max_long=maxlon,
					min_lat=minlat,
					min_long=minlon
			where effortid=eid;
			
		return eid;
	end;
'language 'plpgsql';

