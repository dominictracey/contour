--
-- draw.sql - 
--   This program is free software. You can use, copy and modify it under
-- the terms of the GPL. See the file COPYING for the full GPL license.
--
-- (c) 2000 Aquilon Technology Systems, Inc.
--
-- Created: Wed Dec 12 21:59:54 EST 2001
--

-- I am going to factor out fisheries specific information. This is for dragging.


drop table effort_draw;
create table effort_draw
(
	effortid integer references effort(effortid)
);

--
--- effort_draw_start - Create a new "scribble". This "inherits" from effort
---  so create one of them first and just include a reference to it.
--
drop function effort_draw_start();
create function effort_draw_start()
	returns int4 as '
	declare
		eid int4;
	begin
		select effort_start() into eid;
		insert into effort_draw (effortid) values
						 (eid);
		return eid;
	end;
'language 'plpgsql';

--
--- effort_draw_end - Again, not so interesting, just passes through call to base class.
---    @REX - we should prolly check that the passed in id is actually a draw_effort.
--
drop function effort_draw_end(int4);
create function effort_draw_end(int4)
	returns int4 as '
	declare
		eid alias for $1;
	begin
		select effort_end(eid) into eid;
		return eid;
	end;
'language 'plpgsql';
