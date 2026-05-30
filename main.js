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
