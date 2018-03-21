pragma foreign_keys = on;

create table info (
       id text,
       version text );

create table nodetypes (
       typ integer primary key,
       descr text );

create table trees (
       tid integer primary key,
       tname text );

create table nodes (
       nid integer primary key,
       tid integer,
       typ integer,
       x integer,
       y integer,
       z integer,
       foreign key(tid) references trees(tid)
       	       on delete restrict
       );
       
create table tags (
       tagid integer primary key,
       x integer,
       y integer,
       z integer,
       tag text );

create table simplesynapses (
       -- Straightforward one to one synapses
       sid integer primary key,
       prenid integer,
       postnid integer,
       foreign key(prenid) references nodes(nid)
       	       on delete propagate,
       foreign key(postnid) references nodes(nid)
       	       on delete propagate
       );

create table nodecons (
       -- Part of a neuronal process
       ncid integer primary key,
       nid1 integer,
       nid2 integer,
       foreign key(nid1) references nodes(nid)
       	       on delete cascade
       	       on update cascade,
       foreign key(nid2) references nodes(nid)
       	       on delete cascade
       	       on update cascade );

create table polysynapses (
       -- Polyadic synapses
       sid integer primary key
       );

create table presyncon (
       -- Presynaptic terminals for polyadic synapses
       scid integer primary key,
       sid integer,
       prenid integer,
       foreign key(sid) references polysynapses(sid)
       	       on delete cascade
       	       on update cascade,
       foreign key(prenid) references nodes(nid)
       	       on delete cascade
       	       on update cascade );

create table postsyncon (
       -- Postsynaptic terminals for polyadic synapses
       scid integer primary key,
       sid integer,
       postnid integer,
       foreign key(sid) references polysynapses(sid)
       	       on delete cascade
       	       on update cascade,
       foreign key(postnid) references nodes(nid)
       	       on delete cascade
       	       on update cascade );

----------------------------------------------------------------------

create index if not exists zidx on nodes(z);
create index if not exists treeidx on nodes(tid);

----------------------------------------------------------------------

insert into info values("sbemviewer", "0.1");

----------------------------------------------------------------------

insert into nodetypes values(1, "Soma");
insert into nodetypes values(2, "ExitPoint");
insert into nodetypes values(3, "TreeNode");
insert into nodetypes values(5, "PresynTerm");
insert into nodetypes values(6, "PostsynTerm");
	       
