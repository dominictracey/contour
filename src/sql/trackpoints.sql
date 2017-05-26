--
-- trackpoints.sql - 
--   This program is free software. You can use, copy and modify it under
-- the terms of the GPL. See the file COPYING for the full GPL license.
--
-- (c) 2000 Aquilon Technology Systems, Inc.
--
-- Created: Sun Jul 29 23:33:56 EDT 2001
--

drop table trackpoints;
create table trackpoints
(
	trackpointid integer not null primary key,
	latitude float8 not null,
	longitude float8 not null,
	depth float4,
	date_last_sampled date,
	time_last_sampled time
);

drop sequence seq_trackpoints;
create sequence seq_trackpoints start 1;

--
--- trackpoint_effort - associative table between trackpoints and the effort they
---   belong to.
--
drop table trackpoint_effort;
create table trackpoint_effort 
(
	trackpointid integer not null references trackpoints(trackpointid),
	effortid integer not null references effort(effortid)
);

drop index idx_trackpoint_effort_effortid;
create index idx_trackpoint_effort_effortid on trackpoint_effort(effortid);

drop function trackpoint_add(int4, float8, float8, float4);
create function trackpoint_add (int4, float8, float8, float4) returns int4 as '
	declare
		eid alias for $1;
		lat alias for $2;
		lon alias for $3;
		dep alias for $4;
		currtime time;
		currdate date;
		tpid int4;
	begin

		currdate := ''now'';
		currtime := ''now'';
		tpid := nextval(''seq_trackpoints'');
		
		insert into trackpoints (trackpointid, latitude, longitude, depth, date_last_sampled, time_last_sampled)	
			values (tpid,lat,lon,dep,currdate,currtime);

		insert into trackpoint_effort (trackpointid, effortid) values (tpid, eid);
		
		return tpid;
	end;
' language 'plpgsql';
		
