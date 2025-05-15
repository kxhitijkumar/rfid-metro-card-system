# admin_dashboard.py

import streamlit as st
import requests
import pandas as pd

# Supabase configuration
SUPABASE_URL = "https://wdjqxyrtcxzmtceuhoyy.supabase.co/rest/v1/cards"
SUPABASE_API_KEY = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6IndkanF4eXJ0Y3h6bXRjZXVob3l5Iiwicm9sZSI6ImFub24iLCJpYXQiOjE3NDYyODM1NTIsImV4cCI6MjA2MTg1OTU1Mn0.gO_qzxzeUPeT_yBGvY9b3_VVndMc_t5ZcaVwkFjixQE"
HEADERS = {
    "apikey": SUPABASE_API_KEY,
    "Authorization": f"Bearer {SUPABASE_API_KEY}",
    "Content-Type": "application/json"
}

def fetch_cards():
    url = f"{SUPABASE_URL}/cards?select=*"
    res = requests.get(url, headers=HEADERS)
    return res.json()

def add_card(uid, balance):
    payload = {"uid": uid, "balance": balance}
    res = requests.post(f"{SUPABASE_URL}/cards", headers=HEADERS, json=payload)
    return res.status_code == 201

def update_balance(uid, new_balance):
    payload = {"balance": new_balance}
    res = requests.patch(f"{SUPABASE_URL}/cards?uid=eq.{uid}", headers=HEADERS, json=payload)
    return res.status_code == 204

def delete_card(uid):
    res = requests.delete(f"{SUPABASE_URL}/cards?uid=eq.{uid}", headers=HEADERS)
    return res.status_code == 204

# Streamlit UI
st.set_page_config(page_title="RFID Metro Admin Panel", layout="wide")

st.title("RFID Metro Card Admin Dashboard")

tabs = st.tabs(["📄 View Cards", "➕ Add Card", "📝 Update Balance", "🗑 Delete Card"])

with tabs[0]:
    st.subheader("All Cards")
    data = fetch_cards()
    df = pd.DataFrame(data)
    st.dataframe(df)

with tabs[1]:
    st.subheader("Add a New Card")
    uid = st.text_input("UID")
    balance = st.number_input("Initial Balance", min_value=0)
    if st.button("Add Card"):
        if add_card(uid, balance):
            st.success("Card added successfully.")
        else:
            st.error("Failed to add card.")

with tabs[2]:
    st.subheader("Update Balance")
    uid_update = st.text_input("UID to Update")
    new_balance = st.number_input("New Balance", min_value=0)
    if st.button("Update Balance"):
        if update_balance(uid_update, new_balance):
            st.success("Balance updated.")
        else:
            st.error("Update failed.")

with tabs[3]:
    st.subheader("Delete Card")
    uid_delete = st.text_input("UID to Delete")
    if st.button("Delete"):
        if delete_card(uid_delete):
            st.success("Card deleted.")
        else:
            st.error("Deletion failed.")
