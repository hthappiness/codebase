CREATE TABLE if NOT EXISTS tb_engine
(
  engine_id character varying(255) NOT NULL,
  ip character varying(255),
  port integer,
  algorithm_id integer,
  product_type integer,
  service_type integer,
  memory integer,
  gpu integer,
  cpu integer,
  allocated_num integer,
  upper_limit_rate integer,
  current_rate integer,
  create_time integer,
  update_time integer,
  start_time integer,
  CONSTRAINT tb_engine_pkey PRIMARY KEY (engine_id)
);

CREATE TABLE if NOT EXISTS TB_FACE_TASK
(
  task_id character varying(32) NOT NULL,
  task_name character varying(128) NOT NULL,
  camera_id character varying(64) NOT NULL,
  task_type bigint,
  task_status bigint,
  task_param json,
  excep_level bigint,
  continous_excep_times bigint,
  create_time bigint,
  update_time integer,
  engine_info json,
  progress bigint,
  description character varying(128),
  group_id character varying(32),
  algorithm_id_list json,
  source_type integer,
  source_param json,
  CONSTRAINT tb_face_task_pkey PRIMARY KEY (task_id)
);
alter table tb_face_task alter column create_time type bigint;

CREATE TABLE if NOT EXISTS TB_FACE_SUBTASK
(
  parent_task_id character varying(32),
  task_id character varying(32),
  task_name character varying(128),
  camera_id character varying(64),
  task_type integer,
  task_status integer,
  task_param json,
  algorithm_id_list json,
  source_type integer,
  source_param json,
  create_time numeric(32,0),
  update_time numeric(32,0),
  progress integer,
  description character varying(128),
  CONSTRAINT tb_face_subtask_pkey PRIMARY KEY (task_id)
);

CREATE TABLE if NOT EXISTS tb_vehicle_task
(
  task_id character varying(32) NOT NULL,
  task_name character varying(128) NOT NULL,
  camera_id character varying(64) NOT NULL,
  task_type bigint,
  task_status bigint,
  task_param json,
  excep_level bigint,
  continous_excep_times bigint,
  create_time bigint,
  update_time integer,
  engine_info json,
  progress bigint,
  description character varying(128),
  group_id character varying(32),
  algorithm_id_list json,
  source_type integer,
  source_param json,
  CONSTRAINT tb_vehicle_task_pkey1 PRIMARY KEY (task_id)
);
alter table tb_vehicle_task alter column create_time type bigint;

CREATE TABLE if NOT EXISTS tb_vehicle_subtask
(
  parent_task_id character varying(32),
  task_id character varying(32) NOT NULL,
  task_name character varying(128),
  camera_id character varying(64),
  task_type integer,
  task_status integer,
  task_param json,
  algorithm_id_list json,
  source_type integer,
  source_param json,
  create_time numeric(32,0),
  update_time numeric(32,0),
  progress integer,
  group_id character varying(32),
  description character varying(128),
  CONSTRAINT tb_vehicle_subtask_pkey PRIMARY KEY (task_id)
);

CREATE TABLE if NOT EXISTS TB_VS_TASK
(
  task_id character varying(32) NOT NULL,
  task_name character varying(128) NOT NULL,
  camera_id character varying(64) NOT NULL,
  task_type bigint,
  task_status bigint,
  task_param json,
  excep_level bigint,
  continous_excep_times bigint,
  create_time bigint,
  update_time integer,
  engine_info json,
  progress bigint,
  description character varying(128),
  group_id character varying(32),
  algorithm_id_list json,
  source_type integer,
  source_param json,
  CONSTRAINT tb_vs_task_pkey PRIMARY KEY (task_id)
);
alter table tb_vs_task alter column create_time type bigint;

CREATE TABLE if NOT EXISTS TB_VS_SUBTASK
(
  parent_task_id character varying(32),
  task_id character varying(32) NOT NULL,
  task_name character varying(128),
  camera_id character varying(64),
  task_type integer,
  task_status integer,
  task_param json,
  algorithm_id_list json,
  source_type integer,
  source_param json,
  create_time numeric(32,0),
  update_time numeric(32,0),
  progress integer,
  group_id character varying(32),
  description character varying(128),
  CONSTRAINT tb_vs_subtask_pkey PRIMARY KEY (task_id)
);

CREATE TABLE if NOT EXISTS tb_density_task
(
  task_id character varying(32) NOT NULL,
  task_name character varying(128) NOT NULL,
  camera_id character varying(64) NOT NULL,
  task_type bigint,
  task_status bigint,
  task_param json,
  excep_level bigint,
  continous_excep_times bigint,
  create_time bigint,
  update_time integer,
  engine_info json,
  progress bigint,
  description character varying(128),
  group_id character varying(32),
  algorithm_id_list json,
  source_type integer,
  source_param json,
  CONSTRAINT tb_density_task_pkey PRIMARY KEY (task_id)
);

CREATE TABLE if NOT EXISTS tb_density_subtask
(
  parent_task_id character varying(32),
  task_id character varying(32) NOT NULL,
  task_name character varying(128),
  camera_id character varying(64),
  task_type integer,
  task_status integer,
  task_param json,
  algorithm_id_list json,
  source_type integer,
  source_param json,
  create_time numeric(32,0),
  update_time numeric(32,0),
  progress integer,
  group_id character varying(32),
  description character varying(128),
  CONSTRAINT tb_density_subtask_pkey PRIMARY KEY (task_id)
);

CREATE TABLE if NOT EXISTS tb_ps_task
(
  task_id character varying(32) NOT NULL,
  task_name character varying(128) NOT NULL,
  camera_id character varying(64) NOT NULL,
  task_type bigint,
  task_status bigint,
  task_param json,
  excep_level bigint,
  continous_excep_times bigint,
  create_time bigint,
  update_time integer,
  engine_info json,
  progress bigint,
  description character varying(128),
  group_id character varying(32),
  algorithm_id_list json,
  source_type integer,
  source_param json,
  CONSTRAINT tb_ps_task_pkey PRIMARY KEY (task_id)
);
alter table tb_ps_task alter column create_time type bigint;

CREATE TABLE if NOT EXISTS tb_ps_subtask
(
  parent_task_id character varying(32),
  task_id character varying(32) NOT NULL,
  task_name character varying(128),
  camera_id character varying(64),
  task_type integer,
  task_status integer,
  task_param json,
  algorithm_id_list json,
  source_type integer,
  source_param json,
  create_time numeric(32,0),
  update_time numeric(32,0),
  progress integer,
  group_id character varying(32),
  description character varying(128),
  CONSTRAINT tb_ps_subtask_pkey PRIMARY KEY (task_id)
);

create table if NOT EXISTS tb_image_repository (task_id varchar(32),
                                 image_id varchar(32),
                                 product_type varchar(32),
                                 vendor varchar(32),
                                 sdk_version varchar(32),
                                 tag varchar(32),
                                 name varchar(128),
                                 create_time timestamp,
                                 size varchar(16),
                                 service_type integer,
                                 descriptions varchar(512),
                                 progress integer,
                                 status integer,
                                 constraint pk_tb_image_repository_p primary key (product_type, vendor, service_type, sdk_version));
--ALTER TABLE "public"."tb_image_repository" ADD CONSTRAINT "tb_image_repository_pkey" PRIMARY KEY ("task_id");
CREATE SEQUENCE if NOT EXISTS tb_image_repository_task_id_seq START WITH 1 INCREMENT BY 1 NO MINVALUE NO MAXVALUE CACHE 1;
alter table tb_image_repository alter column task_id set default nextval('tb_image_repository_task_id_seq');

CREATE TABLE if NOT EXISTS tb_trace_task
( 
  DispositionID varchar(64),
  Orientation integer,
  FeatureData character varying(2048),
  ObjectID integer,
  ImgData text,
  CONSTRAINT tb_trace_task PRIMARY KEY (DispositionID)
);