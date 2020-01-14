

topicslist=$(kafka-topics.sh --list --zookeeper zookeeper-cs:2181)
if [ $? -ne 0 ]; then
  echo "cannot get the topic list"
  exit 1
fi

check_and_add()
{
  if [ -z $1 ]; then
    echo "the topic is empty"
    return 1
  fi

  for topic in ${topicslist}
  do
    if [ ${topic} = $1 ]; then
      echo "the topic $1 already exist"
      return 2  
    fi
  done

  kafka-topics.sh --create --zookeeper zookeeper-cs:2181 --replication-factor 1 --partitions $2 --topic $1  
  return $?
}

#资源中心保活，资源资源中心为消费者，分析节点为生产者
check_and_add "aix_res_keepalive_topic" 1

#任务调度保活，任务调度中心为消费者，分析节点为生产者
check_and_add "aix_task_keepalive_topic" 1

#人脸图片流任务同步，任务调度为生产者，每个人脸图片流的pod单独为一个group，分区为1即可
check_and_add "aix_face_task_sync_topic" 1

#车辆图片流任务同步，任务调度为生产者，每个车辆图片流的pod单独为一个group，分区为1即可
check_and_add "aix_vehicle_task_sync_topic" 1

#结构化图片流任务同步，任务调度为生产者，每个结构化图片流的pod单独为一个group，分区为1即可
check_and_add "aix_ps_task_sync_topic" 1

#资源中心异常，任务调度中心为消费者，资源中心为生产者
check_and_add "aix_kafka_engine_abnormal_topic" 1

#任务异常上报
check_and_add "aix_health_report_topic" 1

#人脸与车辆数据的订阅与发布，需与viid保持一致
check_and_add "viid_snapface_topic" 60
check_and_add "viid_ia_snapface_topic" 60
check_and_add "viid_ia_vehicle_topic" 60
check_and_add "viid_vehicle_topic" 60
check_and_add "viid_ia_nonvehicle_topic" 60
check_and_add "viid_nonvehicle_topic" 60
check_and_add "viid_ia_person_topic" 60
check_and_add "viid_person_topic" 60

#人脸短码发布，作为生产者，需与viid保持一致
check_and_add "vias_short_code_topic" 60
check_and_add "vias_fps_short_code_topic" 60
#人流密度
check_and_add "viid_videolabel_topic" 1