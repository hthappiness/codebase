SELECT pg_terminate_backend(pg_stat_activity.pid)
FROM pg_stat_activity
WHERE datname='vias' AND pid<>pg_backend_pid();

drop table if exists tb_vehicle_task; 
drop table if exists tb_obj_extraction_task; 
drop table if exists tb_image_repository; 
drop table if exists tb_face_task; 
drop table if exists tb_face_library_task; 
drop table if exists tb_face_library_info; 
drop table if exists tb_face_libid_vender_version; 
drop table if exists tb_face_info; 
drop table if exists tb_engine;
drop table if exists tb_license;
drop table if exists tb_face_10001;
drop table if exists tb_face_feature_10001;
drop table if exists tb_face_subtask;
drop table if exists tb_vehicle_subtask;
drop table if exists tb_vs_subtask;
drop table if exists tb_vs_task;
drop table if exists tb_density_task;
drop table if exists tb_density_subtask;
drop table if exists tb_ps_task;
drop table if exists tb_ps_subtask;
drop database if exists vias;