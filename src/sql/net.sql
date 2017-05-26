--
-- net.sql - 
--  frame_type is something like 1 - rock-hoppers, 2 - rollers
--
--   This program is free software. You can use, copy and modify it under
-- the terms of the GPL. See the file COPYING for the full GPL license.
--
-- (c) 2000 Aquilon Technology Systems, Inc.
--
-- Created: Wed Aug 22 05:55:26 EDT 2001
--

-- The frame type describes the components used in the building of the frame of the
--  net.
drop table net_frame_type;
create table net_frame_type
(
	net_frame_type_id int4 not null primary key,
	short_name varchar not null,
	description varchar
);

drop sequence seq_net_frame_type;
create sequence seq_net_frame_type start 1;

insert into net_frame_type 
	(net_frame_type_id, short_name, description) 
	values (nextval('seq_net_frame_type'), 'Roller', 'Roller frame over chain or wire');

insert into net_frame_type 
	(net_frame_type_id, short_name, description) 
	values (nextval('seq_net_frame_type'), 'Rock Hopper', 'Non-rolling components over chain or wire');
	
drop table nets;

-- @REX Does Postgres support FOREIGN KEY constraint yet? (no as of 6.5)
create table nets 
(
	netid integer not null primary key,
	name varchar not null,
	description varchar,
	footrope_length int4,
	headrope_length int4,
	legs bool,
	meshes_across_belly int4,
	meshes_down_belly int4,
	frame_type int4,
	frame_size_inches int4,
	date_built date,
	builder varchar,
	active bool,
	date_entered date,
	date_updated date
);

drop sequence seq_nets;
create sequence seq_nets start 1;

drop function net_add(varchar,varchar,int4,int4,bool,int4,int4,int4,int4,date,varchar);
create function net_add(varchar,varchar,int4,int4,bool,int4,int4,int4,int4,date,varchar)
	returns int4 as '
	declare
		nm alias for $1;
		desc alias for $2;		
		fr_len alias for $3;		
		hr_len alias for $4;
		lgs alias for $5;
		mab alias for $6;
		mdb alias for $7;
		ft alias for $8;
		fsi alias for $9;
		db alias for $10;
		bldr alias for $11;
		currdate date;
	begin
		currdate := ''now'';
		insert into nets (netid, name, description, footrope_length, headrope_length,
							legs, meshes_across_belly, meshes_down_belly, frame_type,
							frame_size_inches, date_built, builder, active, date_entered,
							date_updated) values
						 (nextval(''seq_nets''),nm, desc, fr_len, hr_len, 
						 lgs, mab, mdb, ft, fsi, db, bldr, true, currdate, null);
		return currval(''seq_nets'');
	end;
'language 'plpgsql';

drop function net_edit(int4, varchar,varchar,int4,int4,bool,int4,int4,int4,int4,date,varchar,bool);
create function net_edit(int4, varchar,varchar,int4,int4,bool,int4,int4,int4,int4,date,varchar,bool)
	returns int4 as '
	declare
		id alias for $1;
		n alias for $2;
		desc alias for $3;		
		fr_len alias for $4;		
		hr_len alias for $5;
		lgs alias for $6;
		mab alias for $7;
		mdb alias for $8;
		ft alias for $9;
		fsi alias for $10;
		db alias for $11;
		bldr alias for $12;
		act alias for $13;
		currdate date;
	begin
		currdate := ''now'';
		update nets set name=n, 
						description=desc, 
						footrope_length=fr_len, 
						headrope_length=hr_len,
						legs=lgs, 
						meshes_across_belly=mab, 
						meshes_down_belly=mdb, 
						frame_type=ft,
						frame_size_inches=fsi, 
						date_built=db, 
						builder=bldr, 
						date_updated=currdate,
						active=act
					where netid=id;
		return id;
	end;
'language 'plpgsql';
						 	
