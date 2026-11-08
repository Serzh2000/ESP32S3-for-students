// Панель, шаг 3 (фрагмент): оформление. Блок <style> вставляется в PAGE между </title> и </head>.
<style>
  body  { font-family: sans-serif; background: #eef2f7;
          display: flex; justify-content: center; padding-top: 40px; }
  .card { background: white; border-radius: 20px; padding: 24px 32px; width: 260px;
          box-shadow: 0 6px 20px rgba(0,0,0,.12); text-align: center; }
  h1    { color: #7b241c; font-size: 24px; }
  .led  { width: 90px; height: 90px; border-radius: 50%; margin: 10px auto 20px;
          background: #ccc; transition: .3s; }
  .led.on { background: #ff5a4f; box-shadow: 0 0 30px #ff5a4f; }
  button { background: #1f618d; color: white; border: none; border-radius: 12px;
           font-size: 18px; padding: 10px 28px; }
  input  { width: 100%; margin-top: 20px; }
</style>
