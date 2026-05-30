

/*
 * Archivo web: main.js
 * Papel técnico: facsímil estático del runtime visual. Este JavaScript no comparte estado con Tlalpowa.exe; sólo conserva una maqueta interactiva mínima para validar navegación superficial, pestañas y conmutadores sin tocar datos reales.
 * La separación evita que una vista web decorativa se confunda con el motor analítico de escritorio.
 */
(function () {
  "use strict";

/*
 * Función setText: encapsula una interacción pequeña del facsímil web y debe permanecer sin acceso a archivos, red o datos internos. Su efecto lateral se limita al DOM ya cargado.
 */
  function setText(selector, text) {
    var node = document.querySelector(selector);
    if (node) node.textContent = text;
  }

/*
 * Función initTabs: encapsula una interacción pequeña del facsímil web y debe permanecer sin acceso a archivos, red o datos internos. Su efecto lateral se limita al DOM ya cargado.
 */
  function initTabs() {
    var tabs = Array.prototype.slice.call(document.querySelectorAll(".tab:not(.add)"));
    tabs.forEach(function (tab) {
      tab.addEventListener("click", function () {
        tabs.forEach(function (t) { t.classList.remove("active"); });
        tab.classList.add("active");
      });
    });
  }

/*
 * Función initLayerToggles: encapsula una interacción pequeña del facsímil web y debe permanecer sin acceso a archivos, red o datos internos. Su efecto lateral se limita al DOM ya cargado.
 */
  function initLayerToggles() {
    Array.prototype.slice.call(document.querySelectorAll(".layer")).forEach(function (layer) {
      layer.addEventListener("click", function () {
        layer.classList.toggle("on");
      });
    });
  }

/*
 * Función initBuildStamp: encapsula una interacción pequeña del facsímil web y debe permanecer sin acceso a archivos, red o datos internos. Su efecto lateral se limita al DOM ya cargado.
 */
  function initBuildStamp() {
    var stamp = document.body ? document.body.getAttribute("data-build") : "";
    if (stamp) setText(".note", "Facsímil generado: " + stamp);
  }

  document.addEventListener("DOMContentLoaded", function () {
    initTabs();
    initLayerToggles();
    initBuildStamp();
  });
}());
