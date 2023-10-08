#include <stdio.h>
#include <stdint.h> // have to use good habits
#include <stdbool.h>

int main (){

// Email address

  int32_t email, domain, validEmail;
  bool emailPass = false, localFail = false, domainFail = false;
  
  printf("Please enter the email address\t     > ");
  scanf("%*[a-zA-Z0-9]@%n%*[ntnu.edu.tw|gapps.ntnu.edu.tw|csie.ntnu.edu.tw|csie.cool]%n", &email, &domain);
  validEmail = domain - email;
    if (validEmail == 11 || validEmail == 17 || validEmail == 16 || validEmail == 9){
    emailPass = true;
    }
    else if (email == 0) {
    localFail = true;
    }
    else {
    domainFail = true;
    }

// Email Subject

	int32_t gen, gentext = 0, hwtext = 0, hw = 0, p = 0;
	bool titleFail = false, categoryFail = false, generalPass = false, hwPass = false; 
 	printf("Please enter the email subject\t     > ");
	scanf("%d", &gen);
	/*if (gen > 0 && hw == 0 && p == 0){
	printf("passed %d\n", gen);
	}
	else if (hw > 0 && hw < 10 && p < 10 && p > 0 && gen == 0){
	printf("passed %d %d\n", hw, p);
	}
	else if (gen == 0 || hw < 0 || pw < 0) {
	printf("no category\n");
	}
	else {
	printf("no title\n");
	}*/

// Email Content score

  int32_t content;
  bool correctContent = false, rangeFail = false, lowFail = false;
  printf("\nPlease enter the email content score > ");
  scanf("%d", &content);
  if (content < 0 && content < 10 ^ 19){
  	if (content < gentext * (10)^19 || content < hwtext * (10)^19){
  	lowFail = true;
  	}
  	else {
  	rangeFail == true;
  	}
  }
  else {
  correctContent = true;
  }
  
printf("================================================================================\n\n");

int32_t Acceptance = 0;
bool firstFail = false, secondFail = false;

	if (emailPass == 1){
	printf("Sender Address Test : Passed\n");
	Acceptance += 1;
	}
	else if (localFail == 1){
	printf("Sender Address Test : Failed, local-part is invalid\n");
	}
	else if (domainFail == 1){
	printf("Sender Address Test : Failed, domain is not authorized\n");
	}
	
	if (Acceptance == 1){
	if (titleFail == 1){
	printf("Email Subject Test : Failed , no title\n");
	}
	else if (categoryFail == 1){
	printf("Email Subject Test : Failed , no category");
	}
	else if (generalPass == 1){
	printf("Email Subject Test : Passed\n");
	Acceptance += 1;
	}
	else if (hwPass == 1){
	printf("Email Subject Test : Passed\n");
	Acceptance += 1;
	}
	}
	else {
	printf("Email Subject Test : -\n");
	firstFail = true;
	}
	
	if (Acceptance == 2){
	if (correctContent == 1){
	printf("Email Content Test : Passed\n");
	Acceptance += 1;
	}
	else if (rangeFail == 1){
	printf("Email Content Test : Failed, out of range\n");
	}
	else if (lowFail == 1){
	printf("Email Content Test : Failed, too low\n");
	} 
	}
	else  if (Acceptance <= 2){
	printf("Email Content Test : -\n");
	secondFail = true;
	}
	
printf("--------------------------------------------------------------------------------\n");

if (Acceptance == 3 && firstFail == 0 && secondFail == 0){
	if (generalPass == 1){
	printf("\nAssigned to TA QB\n");
	return 0;
	}
	else {
	int32_t c = (hw * p) % 5;
	switch (c) {
	
	case 1:
	printf("\nAssigned to Kaname Madoka\n");
	return 0;
	break;
	
	case 2:
	printf("\nAssigned to Akemi Homura\n");
	return 0;
	break;
	
	case 3:
	printf("\nAssigned to Miki Sayaka\n");
	return 0;
	break;
	
	case 4:
	printf("\nAssigned to Tomoe Mami\n");
	return 0;
	break;
	
	case 5:
	printf("\nAssigned to Sakura Kyoko\n");
	return 0;
	break;
	}
	}
}
else {
printf("\nRejected\n");
return 0;
}
  return 0;
}
