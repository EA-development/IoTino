package jcbs.eadev.iotino;

import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.data.jpa.repository.config.EnableJpaAuditing;

@SpringBootApplication
@EnableJpaAuditing
public class IoTinoApplication {

	public static void main(String[] args) {
		SpringApplication.run(IoTinoApplication.class, args);
	}
}
