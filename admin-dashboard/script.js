const SUPABASE_URL = 'https://wdjqxyrtcxzmtceuhoyy.supabase.co';
const SUPABASE_KEY = '<eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6IndkanF4eXJ0Y3h6bXRjZXVob3l5Iiwicm9sZSI6ImFub24iLCJpYXQiOjE3NDYyODM1NTIsImV4cCI6MjA2MTg1OTU1Mn0.gO_qzxzeUPeT_yBGvY9b3_VVndMc_t5ZcaVwkFjixQE>';
const TABLE = 'cards';

const headers = {
  apikey: SUPABASE_KEY,
  Authorization: `Bearer ${SUPABASE_KEY}`,
  'Content-Type': 'application/json',
};

async function loadCards() {
  const res = await fetch(`${SUPABASE_URL}/rest/v1/${TABLE}?select=*`, {
    headers
  });
  const cards = await res.json();

  const tableBody = document.querySelector('#cardTable tbody');
  tableBody.innerHTML = '';

  cards.forEach(card => {
    const row = document.createElement('tr');
    row.innerHTML = `
      <td>${card.uid}</td>
      <td>${card.balance}</td>
      <td>
        <button onclick="deleteCard('${card.uid}')">Delete</button>
      </td>
    `;
    tableBody.appendChild(row);
  });
}

async function deleteCard(uid) {
  await fetch(`${SUPABASE_URL}/rest/v1/${TABLE}?uid=eq.${uid}`, {
    method: 'DELETE',
    headers
  });
  loadCards();
}

document.querySelector('#cardForm').addEventListener('submit', async (e) => {
  e.preventDefault();
  const uid = document.querySelector('#uid').value;
  const balance = parseInt(document.querySelector('#balance').value);

  await fetch(`${SUPABASE_URL}/rest/v1/${TABLE}`, {
    method: 'POST',
    headers,
    body: JSON.stringify({ uid, balance })
  });

  loadCards();
  e.target.reset();
});

loadCards();
