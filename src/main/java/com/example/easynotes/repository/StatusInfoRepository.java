package com.example.easynotes.repository;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import com.example.easynotes.model.StatusInfo;

@Repository
public interface StatusInfoRepository extends JpaRepository<StatusInfo, Long> {

}
