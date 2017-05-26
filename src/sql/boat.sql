--
-- boat.sql - 
--   This program is free software. You can use, copy and modify it under
-- the terms of the GPL. See the file COPYING for the full GPL license.
--
-- (c) 2000 Aquilon Technology Systems, Inc.
--
-- Created: Sat Feb 16 10:05:28 EST 2002
--

drop table vessel;

create table vessel
(
	vesselid integer not null primary key,
	name varchar not null,
	owner varchar not null,
	registration_num varchar,
	horsepower int4,
	LOA int4
);

insert into vessel (vesselid, name, owner, registration_num, horsepower, LOA)
	values ( 1,'prowler','New Meadow''s Trawlers','DPT678',650,82);

