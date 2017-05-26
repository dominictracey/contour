--
-- catch.sql - 
--   This program is free software. You can use, copy and modify it under
-- the terms of the GPL. See the file COPYING for the full GPL license.
--
-- (c) 2002 Aquilon Technology Systems, Inc.
--
-- Created: Tue Feb 12 22:50:19 EST 2002
--

drop table disposition;

create table disposition
(
	dispid integer not null primary key,
	name varchar
);

insert into disposition (dispid,name)
	values (1,'kept');

insert into disposition (dispid,name)
	values (2,'discard');

drop table catch;

-- a row in the catch table represents the amount of one cull of one specie
--	caught during one tow.
create table catch
(
	catchid integer not null primary key,
	effortid integer references effort(effortid),
	cullid integer references cull(cullid),
	dispid integer references disposition(dispid),
	weight integer,
	unique (effortid,cullid)
);

drop sequence seq_catch;
create sequence seq_catch start 1;

drop function catch_add(int4,int4,int4,int4);
create function catch_add(int4,int4,int4,int4)
        returns int4 as '
        declare
                eid alias for $1;
                cull alias for $2;
                dis alias for $3;
                wt alias for $4;
        begin
                insert into catch (catchid, effortid, cullid, dispid, weight)
			values (nextval(''seq_catch''), eid, cull, dis, wt);
                return currval(''seq_catch'');
        end;
'language 'plpgsql';

drop function catch_edit(int4,int4,int4,int4);
create function catch_edit(int4,int4,int4,int4)
        returns int4 as '
        declare
                eid alias for $2;
                cull alias for $3;
                dis alias for $4;
                wt alias for $4;
        begin
                update catch set dispid=dis, weight=wt 
			where effortid=eid and cullid=cull;	
                return cid;
        end;
'language 'plpgsql';

drop function check_catch();
create function check_catch () returns opaque as '
        begin
            -- Check that the weight is a number and is non-negative 
            if new.weight < 0 then
                raise exception ''Weight cannot be a negative value'';
            end if;
            return new;
        end;
' language 'plpgsql';

create trigger check_catch before insert or update on catch 
        for each row execute procedure check_catch();
    


