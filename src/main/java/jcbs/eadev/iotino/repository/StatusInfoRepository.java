package jcbs.eadev.iotino.repository;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import jcbs.eadev.iotino.model.StatusInfo;

@Repository
public interface StatusInfoRepository extends JpaRepository<StatusInfo, Long> {

}
