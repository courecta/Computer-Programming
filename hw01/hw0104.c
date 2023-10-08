#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>



int main (){

// Input format and checker note that the letters a to m are for the 1st to 13th card cause typing is hard :(

  int32_t a, b, c, d, e, f, g, h, i, j, k, l, m;
  int32_t ace = 0, king = 0, queen = 0, jack = 0;
  int32_t spades = 0, hearts = 0, diamonds = 0, clubs = 0;

  printf("1st card: ");
  scanf("%d", &a);
  switch (a) {

    case 1 ... 13:
       spades += 1;
       break;
    case 14 ... 26:
      hearts += 1;;
      break;
    case 27 ... 39:
      diamonds += 1;
      break;
    case 40 ... 52:
      clubs += 1;
      break;
    default:
    printf("Invalid Play\n");
    return 1;
  }
  if (a == 1 || a == 14 || a == 27 || a == 40) {
    ace += 1;
  }
  else if (a % 13 == 0) {
    king += 1;
  }
  else if (a == 12 || a == 25 || a == 38 || a == 51){
    queen += 1;
  }
  else if (a == 11 || a == 24 || a == 37 || a == 50 ) {
    jack += 1;
  }
  
  printf("2nd card: ");
  scanf("%d", &b);
  switch (b) {

    case 1 ... 13:
       spades += 1;
       break;
    case 14 ... 26:
      hearts += 1;;
      break;
    case 27 ... 39:
      diamonds += 1;
      break;
    case 40 ... 52:
      clubs += 1;
      break;
    default:
    printf("Invalid Play\n");
    return 1;
  }
  if (b == 1 || b == 14 || b == 27 || b == 40) {
    ace += 1;
  }
  else if (b % 13 == 0) {
    king += 1;
  }
  else if (b == 12 || b == 25 || b == 38 || b == 51){
    queen += 1;
  }
  else if (b == 11 || b == 24 || b == 37 || b == 50 ) {
    jack += 1;
  }
  
  printf("3rd card: ");
  scanf("%d", &c);
  switch (c) {

    case 1 ... 13:
       spades += 1;
       break;
    case 14 ... 26:
      hearts += 1;;
      break;
    case 27 ... 39:
      diamonds += 1;
      break;
    case 40 ... 52:
      clubs += 1;
      break;
    default:
    printf("Invalid Play\n");
    return 1;
  }
  if (c == 1 || c == 14 || c == 27 || c == 40) {
    ace += 1;
  }
  else if (c % 13 == 0) {
    king += 1;
  }
  else if (c == 12 || c == 25 || c == 38 || c == 51){
    queen += 1;
  }
  else if (c == 11 || c == 24 || c == 37 || c == 50 ) {
    jack += 1;
  }
  
  printf("4th card: ");
  scanf("%d", &d);
  switch (d) {

    case 1 ... 13:
       spades += 1;
       break;
    case 14 ... 26:
      hearts += 1;;
      break;
    case 27 ... 39:
      diamonds += 1;
      break;
    case 40 ... 52:
      clubs += 1;
      break;
    default:
    printf("Invalid Play\n");
    return 1;
  }
  if (d == 1 || d == 14 || d == 27 || d == 40) {
    ace += 1;
  }
  else if (d % 13 == 0) {
    king += 1;
  }
  else if (d == 12 || d == 25 || d == 38 || d == 51){
    queen += 1;
  }
  else if (d == 11 || d == 24 || d == 37 || d == 50 ) {
    jack += 1;
  }
  
  printf("5th card: ");
  scanf("%d", &e);
  switch (e) {

    case 1 ... 13:
       spades += 1;
       break;
    case 14 ... 26:
      hearts += 1;;
      break;
    case 27 ... 39:
      diamonds += 1;
      break;
    case 40 ... 52:
      clubs += 1;
      break;
    default:
    printf("Invalid Play\n");
    return 1;
  }
  if (e == 1 || e == 14 || e == 27 || e == 40) {
    ace += 1;
  }
  else if (e % 13 == 0) {
    king += 1;
  }
  else if (e == 12 || e == 25 || e == 38 || e == 51){
    queen += 1;
  }
  else if (e == 11 || e == 24 || e == 37 || e == 50 ) {
    jack += 1;
  }
  
  printf("6th card: ");
  scanf("%d", &f);
switch (f) {

    case 1 ... 13:
       spades += 1;
       break;
    case 14 ... 26:
      hearts += 1;;
      break;
    case 27 ... 39:
      diamonds += 1;
      break;
    case 40 ... 52:
      clubs += 1;
      break;
    default:
    printf("Invalid Play\n");
    return 1;
  }
  if (f == 1 || f == 14 || f == 27 || f == 40) {
    ace += 1;
  }
  else if (f % 13 == 0) {
    king += 1;
  }
  else if (f == 12 || f == 25 || f == 38 || f == 51){
    queen += 1;
  }
  else if (f == 11 || f == 24 || f == 37 || f == 50 ) {
    jack += 1;
  }
  
  printf("7th card: ");
  scanf("%d", &g);
  switch (g) {

    case 1 ... 13:
       spades += 1;
       break;
    case 14 ... 26:
      hearts += 1;;
      break;
    case 27 ... 39:
      diamonds += 1;
      break;
    case 40 ... 52:
      clubs += 1;
      break;
    default:
    printf("Invalid Play\n");
    return 1;
  }
  if (g == 1 || g == 14 || g == 27 || g == 40) {
    ace += 1;
  }
  else if (g % 13 == 0) {
    king += 1;
  }
  else if (g == 12 || g == 25 || g == 38 || g == 51){
    queen += 1;
  }
  else if (g == 11 || g == 24 || g == 37 || g == 50 ) {
    jack += 1;
  }
  
  printf("8th card: ");
  scanf("%d", &h);
  switch (h) {

    case 1 ... 13:
       spades += 1;
       break;
    case 14 ... 26:
      hearts += 1;;
      break;
    case 27 ... 39:
      diamonds += 1;
      break;
    case 40 ... 52:
      clubs += 1;
      break;
    default:
    printf("Invalid Play\n");
    return 1;
  }
  if (h == 1 || h == 14 || h == 27 || h == 40) {
    ace += 1;
  }
  else if (h % 13 == 0) {
    king += 1;
  }
  else if (h == 12 || h == 25 || h == 38 || h == 51){
    queen += 1;
  }
  else if (h == 11 || h == 24 || h == 37 || h == 50 ) {
    jack += 1;
  }
  
  printf("9th card: ");
  scanf("%d", &i);
switch (i) {

    case 1 ... 13:
       spades += 1;
       break;
    case 14 ... 26:
      hearts += 1;;
      break;
    case 27 ... 39:
      diamonds += 1;
      break;
    case 40 ... 52:
      clubs += 1;
      break;
    default:
    printf("Invalid Play\n");
    return 1;
  }
  if (i == 1 || i == 14 || i == 27 || i == 40) {
    ace += 1;
  }
  else if (i % 13 == 0) {
    king += 1;
  }
  else if (i == 12 || i == 25 || i == 38 || i == 51){
    queen += 1;
  }
  else if (i == 11 || i == 24 || i == 37 || i == 50 ) {
    jack += 1;
  }
  
  printf("10th card: ");
  scanf("%d", &j);
  switch (j) {

    case 1 ... 13:
       spades += 1;
       break;
    case 14 ... 26:
      hearts += 1;;
      break;
    case 27 ... 39:
      diamonds += 1;
      break;
    case 40 ... 52:
      clubs += 1;
      break;
    default:
    printf("Invalid Play\n");
    return 1;
  }
  if (j == 1 || j == 14 || j == 27 || j == 40) {
    ace += 1;
  }
  else if (j % 13 == 0) {
    king += 1;
  }
  else if (j == 12 || j == 25 || j == 38 || j == 51){
    queen += 1;
  }
  else if (j == 11 || j == 24 || j == 37 || j == 50 ) {
    jack += 1;
  }
  
  printf("11th card: ");
  scanf("%d", &k);
  switch (k) {

    case 1 ... 13:
       spades += 1;
       break;
    case 14 ... 26:
      hearts += 1;;
      break;
    case 27 ... 39:
      diamonds += 1;
      break;
    case 40 ... 52:
      clubs += 1;
      break;
    default:
    printf("Invalid Play\n");
    return 1;
  }
  if (k == 1 || k == 14 || k == 27 || k == 40) {
    ace += 1;
  }
  else if (k % 13 == 0) {
    king += 1;
  }
  else if (k == 12 || k == 25 || k == 38 || k == 51){
    queen += 1;
  }
  else if (k == 11 || k == 24 || k == 37 || k == 50 ) {
    jack += 1;
  }
  
  printf("12th card: ");
  scanf("%d", &l);
  switch (l) {

    case 1 ... 13:
       spades += 1;
       break;
    case 14 ... 26:
      hearts += 1;;
      break;
    case 27 ... 39:
      diamonds += 1;
      break;
    case 40 ... 52:
      clubs += 1;
      break;
    default:
    printf("Invalid Play\n");
    return 1;
  }
  if (l == 1 || l == 14 || l == 27 || l == 40) {
    ace += 1;
  }
  else if (l % 13 == 0) {
    king += 1;
  }
  else if (l == 12 || l == 25 || l == 38 || l == 51){
    queen += 1;
  }
  else if (l == 11 || l == 24 || l == 37 || l == 50 ) {
    jack += 1;
  }
  
  printf("13th card: ");
  scanf("%d", &m);
  switch (m) {

    case 1 ... 13:
       spades += 1;
       break;
    case 14 ... 26:
      hearts += 1;;
      break;
    case 27 ... 39:
      diamonds += 1;
      break;
    case 40 ... 52:
      clubs += 1;
      break;
    default:
    printf("Invalid Play\n");
    return 1;
  }
  if (m == 1 || m == 14 || m == 27 || m == 40) {
    ace += 1;
  }
  else if (m % 13 == 0) {
    king += 1;
  }
  else if (m == 12 || m == 25 || m == 38 || m == 51){
    queen += 1;
  }
  else if (m == 11 || m == 24 || m == 37 || m == 50 ) {
    jack += 1;
  }
  
  printf("---\n");

// Calculation Machine

	int32_t acePoints = 0, kingPoints = 0, queenPoints = 0, jackPoints = 0;

	acePoints = ace * 4;
	kingPoints = king * 3;
	queenPoints = queen * 2;
	jackPoints = jack;
	
	int32_t HCP = acePoints + kingPoints + queenPoints + jackPoints;
	
// Output Format

printf("HCP: %d pts\n", HCP);
printf("Suit: %d-%d-%d-%d\n", spades, hearts, diamonds, clubs);

	if (spades == 4 || hearts == 4 || diamonds == 4 || clubs == 4){
		if (11 <= HCP && HCP <= 15){
		printf("The bidding choice : 1NT\n");
		return 0;
		}
		else if (22 <= HCP && HCP <= 24){
		printf("The bidding choice : 2NT\n");
		return 0;
		}
	}
	if (11 <= HCP && HCP <= 15){
		if (spades == 5 || hearts == 5 || diamonds == 5 || clubs == 5 ){
			if ((spades == hearts || spades == diamonds || spades == clubs || hearts == diamonds) &&
			   (hearts == clubs || diamonds == clubs)){
			   printf("The bidding choice : 1♠\n");
			   return 0;
			   }
			   else {
			   printf("The bidding choice : 1♡\n");
			   return 0;
			   }
			}
		}
	 if (11 <= HCP && HCP <= 15 && diamonds >= 4){
	printf("The bidding choice : 1♢\n");
	return 0;
	}
	 if (11 <= HCP && HCP <= 15 && clubs >= 6){
	printf("The bidding choice : 2♣\n");
	return 0;
	}
	if (11 <= HCP && HCP <= 15 && clubs >= 5 && diamonds == 0){
	printf("The bidding choice : 2♢\n");
	return 0;
	}
	if (HCP >= 16){
	printf("The bidding choice : 1♣\n");
	return 0;
	}
	if (spades == 7 || hearts == 7 || diamonds == 7 || clubs == 7){
		if (8 <= HCP && HCP <= 11) {
		printf("The bidding choice : 3♣, 3♢, 3♡, 3♠\n");
		return 0;
		}
	}
	if (8 <= HCP && HCP <= 10){
		if (spades == 6 || hearts == 6){
			if (spades == hearts){
			printf("The bidding choice : 2♠\n");
			return 0;
			}
		}
		else {
		if (spades == hearts){
			printf("The bidding choice : 2♡\n");
			return 0;
			}
		}
	}
	if (HCP < 16){
		if (spades == 7 || hearts == 7){
			if (jack == 0) {
			printf("The bidding choice : 3NT\n");
			return 0;
			}
		}
	}
	else {
	printf("The bidding choice : Pass\n");
	return 0;
	}

  return 0;
}
