/*
 * Archivo web: main.template.js
 * Papel técnico: facsímil estático del runtime visual. Este JavaScript no comparte estado con Tlalpowa.exe; sólo conserva una maqueta interactiva mínima para validar navegación superficial, pestañas y conmutadores sin tocar datos reales.
 * La separación evita que una vista web decorativa se confunda con el motor analítico de escritorio.
 */
(function(){
  const root = document.querySelector('.app');
  const build = root ? root.getAttribute('data-build') : '';
  console.info('Tlalpowa facsímil estático generado durante la compilación:', build);
  document.querySelectorAll('.tab').forEach(tab => {
    tab.addEventListener('click', () => {
      document.querySelectorAll('.tab').forEach(t => t.classList.remove('active'));
      if (!tab.classList.contains('add')) tab.classList.add('active');
    });
  });
})();
