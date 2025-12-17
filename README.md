# 🤖🌱 Plataforma Robótica Autónoma para Campo Agrícola

Proyecto de ingeniería orientado al diseño y desarrollo de una **plataforma robótica autónoma para aplicaciones agrícolas**, enfocada en adquisición de datos, operación en entornos con conectividad limitada y visualización mediante una interfaz web embebida.

El sistema está diseñado con un enfoque **modular y escalable**, permitiendo su adaptación a distintas tareas agrícolas como monitoreo, análisis del entorno y futuras integraciones con inteligencia artificial.
---
## 🎯 Objetivo del proyecto

Desarrollar una plataforma robótica que:

- Adquiera datos del entorno de forma autónoma
- Opere bajo un enfoque **offline-first**
- Almacene información localmente sin pérdida de datos
- Permita la visualización mediante una interfaz web
- Sirva como base para futuras aplicaciones agrícolas inteligentes

---

## 🧠 Enfoque de diseño

El proyecto se desarrolló considerando problemáticas reales del entorno agrícola:

- Conectividad intermitente o inexistente
- Necesidad de sistemas robustos y autónomos
- Bajo consumo energético
- Facilidad de mantenimiento y replicación

Por ello, se eligió una arquitectura centrada en el **ESP32 como sistema embebido principal**, combinando almacenamiento local y conectividad WiFi cuando esté disponible.

---

## 🧩 Arquitectura del sistema

- **Unidad de control:** ESP32 (ESP32-S3)
- **Sensado:** Módulos de sensores (ambientales, distancia, GPS)
- **Almacenamiento:** Tarjeta microSD (CSV / JSON)
- **Comunicación:** WiFi
- **Visualización:** Servidor web embebido (HTML / CSS / JavaScript)

---

## 🛠️ Tecnologías utilizadas

### Hardware
- ESP32 / ESP32-S3
- Módulo microSD
- Sensores ambientales y de distancia
- Sistema de alimentación autónomo

### Software
- C / C++ (Arduino framework)
- HTML, CSS y JavaScript
- Git y GitHub
- Arduino IDE / PlatformIO

---

## 🔄 Funcionamiento general

1. El ESP32 adquiere datos de los sensores en intervalos definidos
2. Los datos se almacenan localmente en la microSD
3. Si existe conexión WiFi:
   - Se habilita el servidor web
   - Los datos pueden visualizarse desde un navegador
4. En ausencia de internet:
   - El sistema continúa registrando datos sin interrupciones
5. Los registros pueden analizarse posteriormente

---

## 🌐 Interfaz web embebida

La interfaz web permite:

- Visualización de datos en tiempo real
- Consulta de registros históricos
- Monitoreo del estado del sistema
- Base para dashboards más avanzados

La página web se aloja directamente en la memoria del ESP32.

---

📈 Resultados actuales

	•	Sistema funcional de adquisición de datos
	•	Operación estable en modo offline
	•	Visualización web embebida
	•	Arquitectura modular lista para expansión

🚀 Mejoras futuras

	•	Integración con GPS y mapeo
	•	Control remoto del robot
	•	Integración con IA para análisis del entorno
	•	Backend externo y dashboards avanzados
	•	Integración con ROS

👨‍💻 Autor

Areivan
Estudiante de Ingeniería en Robótica Industrial
Proyecto académico y experimental enfocado en robótica aplicada












  
