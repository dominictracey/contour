--
-- species.sql - Species and culls. This data model was borrowed from the work
-- 		done by Michael Cahall at ACCSP.
--
--   This program is free software. You can use, copy and modify it under
-- the terms of the GPL. See the file COPYING for the full GPL license.
--
-- (c) 2000 Aquilon Technology Systems, Inc.
--
-- Created: Sun Nov 11 10:34:16 EST 2001
--

drop table species;

create table species
(
	speciesid integer not null primary key,
	common_name varchar,
	scientific_name varchar,
	nmf_code  varchar,
	nmfs_ne_code varchar,
	nodc_code varchar,
	afs_name varchar
);

drop sequence seq_species;
create sequence seq_species start 1;

-- specie_profile - These are like culls of species I think. 
-- drop table specie_profile;
-- 2/12/02 - Don't understand this so I'm going to just do a cull thing for now.
-- create table specie_profile
-- (
-- 	profile_id integer not null primary key,
--  	supergroup_code integer,
-- 	group_code integer,
-- 	profile_name,
--  	supergroup_name,
-- 	group_name
-- );


-- drop sequence seq_profile_specie;
-- create sequence seq_profile_specie start 1;

drop table cull;

create table cull
(
	cullid integer not null primary key,
	name varchar,
	speciesid integer not null references species(speciesid),
	active bool 
);

drop sequence seq_cull;
create sequence seq_cull start 1;

--
--- Cod
--
insert into species (speciesid,common_name,scientific_name,nmf_code,nmfs_ne_code,nodc_code,afs_name)
	values (nextval('seq_species'),'Cod','Greenus Bastardus','909','090','987','Codfish');

insert into cull (cullid, name, speciesid, active)
	values (nextval('seq_cull'),'Large',currval('seq_species'),'t');

insert into cull (cullid, name, speciesid, active)
	values (nextval('seq_cull'),'Scrod',currval('seq_species'),'t');

--
--- Pollox
--
insert into species (speciesid,common_name,scientific_name,nmf_code,nmfs_ne_code,nodc_code,afs_name)
	values (nextval('seq_species'),'Pollock','Longus Blackus','909','090','987','Pollux');

insert into cull (cullid, name, speciesid, active)
	values (nextval('seq_cull'),'Large',currval('seq_species'),'t');

insert into cull (cullid, name, speciesid, active)
	values (nextval('seq_cull'),'Small',currval('seq_species'),'t');

--
--- Haddox
--
insert into species (speciesid,common_name,scientific_name,nmf_code,nmfs_ne_code,nodc_code,afs_name)
	values (nextval('seq_species'),'Haddock','Devilis Thumprintus','909','090','987','Haddux');

insert into cull (cullid, name, speciesid, active)
	values (nextval('seq_cull'),'Large',currval('seq_species'),'t');

insert into cull (cullid, name, speciesid, active)
	values (nextval('seq_cull'),'Small',currval('seq_species'),'t');

--
--- Monkeys
--
insert into species (speciesid,common_name,scientific_name,nmf_code,nmfs_ne_code,nodc_code,afs_name)
	values (nextval('seq_species'),'Monkfish','Bigus Mouthus','909','090','987','Monkeys');

insert into cull (cullid, name, speciesid, active)
	values (nextval('seq_cull'),'Large',currval('seq_species'),'t');

insert into cull (cullid, name, speciesid, active)
	values (nextval('seq_cull'),'Small',currval('seq_species'),'t');

--
--- Hake
--
insert into species (speciesid,common_name,scientific_name,nmf_code,nmfs_ne_code,nodc_code,afs_name)
	values (nextval('seq_species'),'Hake','Hairius Chinius','909','090','987','Hake');

insert into cull (cullid, name, speciesid, active)
	values (nextval('seq_cull'),'Large',currval('seq_species'),'t');

insert into cull (cullid, name, speciesid, active)
	values (nextval('seq_cull'),'Small',currval('seq_species'),'t');

