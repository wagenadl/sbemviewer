pragma foreign_keys = on;

create table info (
       id text,
       version text );

create table nodetypes (
       typ integer primary key,
       descr text );

create table trees (
       tid integer primary key,
       tname text,
       visible boolean,
       cdate date);

create table nodes (
       nid integer primary key,
       tid integer,
       typ integer,
       x integer,
       y integer,
       z integer,
       cdate date,
       foreign key(tid) references trees(tid)
       	       on delete restrict
       );
       
create table tags (
       tagid integer primary key,
       nid integer,
       tag text,
       cdate date,
       foreign key(nid) references nodes(nid)
               on delete cascade
               on update cascade );

create table nodecons (
       -- Part of a neuronal process
       ncid integer primary key,
       nid1 integer,
       nid2 integer,
       cdate date,
       foreign key(nid1) references nodes(nid)
       	       on delete cascade
       	       on update cascade,
       foreign key(nid2) references nodes(nid)
       	       on delete cascade
       	       on update cascade );

create table synapses (
       sid integer primary key,
       cdate date
       );

create table syncons (
       -- Association of pre- and postsynaptic terminals with synapses
       scid integer primary key,
       sid integer,
       nid integer,
       cdate date,
       foreign key(sid) references synapses(sid)
       	       on delete cascade
       	       on update cascade,
       foreign key(nid) references nodes(nid)
       	       on delete cascade
       	       on update cascade );

create table selectedtree (
       tid integer,
       foreign key(tid) references trees(tid)
              on delete set null );

create table selectednode (
       nid integer,
       foreign key(nid) references nodes(nid)
              on delete set null );

----------------------------------------------------------------------

create index if not exists zidx on nodes(z);
create index if not exists treeidx on nodes(tid);

----------------------------------------------------------------------

insert into info values("sbemviewer", "0.2");
insert into selectedtree values(null);
insert into selectednode values(null);

----------------------------------------------------------------------

insert into nodetypes values(1, "Soma");
insert into nodetypes values(2, "ExitPoint");
insert into nodetypes values(3, "TreeNode");
insert into nodetypes values(5, "PresynTerm");
insert into nodetypes values(6, "PostsynTerm");
insert into nodetypes values(7, "SynContour");

