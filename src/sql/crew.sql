---
-- crew.sql - 
--   This program is free software. You can use, copy and modify it under
-- the terms of the GPL. See the file COPYING for the full GPL license.
--
-- (c) 2000 Aquilon Technology Systems, Inc.
--
-- Created: Thu Nov  8 21:30:57 EST 2001
--

drop table crew;
create table crew 
(
	crewid integer not null primary key,
	given_name varchar not null,
	surname varchar not null,
	taxid varchar,
	active bool,
	date_entered date,
	date_updated date
);

drop sequence seq_crew;
create sequence seq_crew start 1;

drop function crew_add(varchar,varchar,varchar);
create function crew_add(varchar,varchar,varchar)
	returns int4 as '
	declare
		fn alias for $1;
		ln alias for $2;		
		ssn alias for $3;		
		currdate date;
	begin
		currdate := ''now'';
		insert into crew (crewid, given_name, surname, taxid, active, date_entered, date_updated) values
						 (nextval(''seq_crew''), fn, ln, ssn, true, currdate, null);
		return currval(''seq_crew'');
	end;
'language 'plpgsql';

drop function crew_edit(int4, varchar,varchar,varchar,bool);
create function crew_edit(int4, varchar,varchar,varchar,bool)
	returns int4 as '
	declare
		id alias for $1;
		fn alias for $2;
		ln alias for $3;		
		ssn alias for $4;		
		act alias for $5;
		currdate date;
	begin
		currdate := ''now'';
		update crew set given_name = fn, 
						surname = ln, 
						taxid = ssn, 
						active = act,
						date_updated = currdate
			where crewid=id;
		return id;
	end;
'language 'plpgsql';

drop function crew_deactivate(int4);
create function crew_deactivate(int4)
	returns int4 as '
	declare
		id alias for $1;
		currdate date;
	begin
		currdate := ''now'';
		update crew set active = false,
						date_updated = currdate
			where crewid=id;
		return id;
	end;
'language 'plpgsql';