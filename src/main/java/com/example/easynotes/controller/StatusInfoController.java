package com.example.easynotes.controller;

import java.util.List;

import javax.validation.Valid;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import com.example.easynotes.model.StatusInfo;
import com.example.easynotes.repository.StatusInfoRepository;

@RestController
@RequestMapping("/api")
public class StatusInfoController {
	@Autowired
	StatusInfoRepository statusInfoRepository;

	@GetMapping("/status")
	public List<StatusInfo> getAllNotes() {
		return statusInfoRepository.findAll();
	}

	@PostMapping("/status")
	public StatusInfo createNote(@Valid @RequestBody StatusInfo statusInfo) {
		return statusInfoRepository.save(statusInfo);
	}

}
