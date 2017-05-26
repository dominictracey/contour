--
-- trip.sql - 
--   This program is free software. You can use, copy and modify it under
-- the terms of the GPL. See the file COPYING for the full GPL license.
--
-- (c) 2000 Aquilon Technology Systems, Inc.
--
-- Created: Sat Feb 16 10:05:48 EST 2002
--

drop table trip;

-- @REX - How do we want to handle the "centralized database" which will hold
--	trip information for multiple boats? If we just let the boats generate
--	their own trip ids, they won't be unique when they are merged. I have put
--	in a uniqueness constraint on the combination of tripid and vesselid, but
--	for this to work we'll need to remove the PK constraint on tripid (since
--	PK implies uniqueness).
create table trip
(
	tripid int4 not null primary key,
	vesselid int4 references vessel(vesselid),
	depart date,
	return date,
	port_depart int4,
	port_return int4,
	days_at_sea int4,
	unique (tripid, vesselid)
);

drop sequence seq_trip;
create sequence seq_trip start 1;

drop function trip_begin(int4);
create function trip_begin(int4)
        returns int4 as '
        declare
                vid alias for $1;
                currdate date;
        begin
                currdate := ''now'';
                insert into trip (tripid,vesselid,depart,return,port_depart,port_return,days_at_sea) values
                                                 (nextval(''seq_trip''), vid, currdate, null, 0, 0, 0);
                return currval(''seq_trip'');
        end;
'language 'plpgsql';

drop function trip_end(int4);
create function trip_end(int4)
        returns int4 as '
        declare
                vid alias for $1;
                currdate date;
        begin
                currdate := ''now'';
                update trip set return=currdate;
                        where tripid=vid;
                return id;
        end;
'language 'plpgsql';

